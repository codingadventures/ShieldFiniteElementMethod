#include "Mesh.h"
#include "kernels.h"


Rendering::Mesh::Mesh(TVecCoord vertices, vector<GLuint> indices, vector<Texture> textures, Material material, vector<glm::vec2> textCoords) : 

	//d_bounding_box(BoundingBox(nullptr)),
	//d_bounding_sphere(BoundingSphere(NULL)),
	d_material(material),
	d_area(0.0f)
{

	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = textures; 
	//this->m_boneWeights = boneWeights; 
	this->m_texCoords = textCoords;
	//this->calculate_center_of_mass();
	//this->calculateArea();
	//this->calculateBoundingBox(); 
	//this->calculate_tex_coord();
	this->setupMesh();
}

void Rendering::Mesh::Draw(Shader& shader)
{
	shader.Use();
	if ( this->m_textures.size()>0)
	{
		for(GLuint i = 0; i < this->m_textures.size(); i++)
		{
			GLuint textureType = GL_TEXTURE_2D;
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			auto uniform_name = m_textures[i].Get_Uniform_Name("1");   
			//if(name == "material.texture_diffuse")
			//	ss << diffuseNr++; // Transfer GLuint to stream
			//else if(name == "material.texture_specular")
			//	ss << specularNr++; // Transfer GLuint to stream
			//number = ss.str(); 
			// Now set the sampler to the correct texture unit
			GLuint shader_location = glGetUniformLocation(shader.m_program,  uniform_name.c_str());
			glUniform1i(shader_location, i);
			// And finally bind the texture
			glBindTexture(textureType, this->m_textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0); // Always good practice to set everything back to defaults once configured.
	}

	d_material.SetShader(shader);

	// Draw mesh
	glBindVertexArray(this->d_VAO);

	//GLuint drawMode = withAdjecencies ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
	GLuint indices_size =  m_indices.size();
	/*	if (withAdjecencies)
	{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->d_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_adjacent_indices.size() * sizeof(GLuint), &this->m_adjacent_indices[0], GL_STATIC_DRAW);

	}*/
	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);
}

void Rendering::Mesh::init(FEMMesh* inputMesh)
{

	if (inputMesh)
	{
		LOGI("Creating mapping between simulation mesh \"%s\" and surface mesh" ,inputMesh->filename.c_str());
		static std::string input_filename;
		static sofa::helper::vector<Mat3x3d> bases;
		static sofa::helper::vector<Vec3d> centers;
		static Octree<Vec3d> octree;
		const TVecTetra& tetras = inputMesh->tetrahedra;
		const TVecCoord& in = inputMesh->positions;
		const TVecCoord& out = m_vertices;
		d_map_i.resize(out.size());
		d_map_f.resize(out.size());
		if (input_filename != inputMesh->filename || bases.size() != tetras.size()) // we have to recompute the octree and bases
		{
			input_filename = inputMesh->filename;
			sofa::helper::vector< BBox<Vec3d> > bbox;
			bases.resize(tetras.size());
			centers.resize(tetras.size());
			bbox.resize(tetras.size());
			LOGI("   Preparing tetrahedra");
			for (unsigned int t=0; t<tetras.size(); ++t)
			{
				Mat3x3d m, mt;
				m[0] = in[tetras[t][1]]-in[tetras[t][0]];
				m[1] = in[tetras[t][2]]-in[tetras[t][0]];
				m[2] = in[tetras[t][3]]-in[tetras[t][0]];
				mt.transpose(m);
				bases[t].invert(mt);
				centers[t] = (in[tetras[t][0]]+in[tetras[t][1]]+in[tetras[t][2]]+in[tetras[t][3]])*0.25;
				bbox[t].add(tetras[t].begin(), tetras[t].end(), in);
			}
			LOGI("  Building octree");
			octree.init(bbox,8,8);
		}
		LOGI( "  Processing vertices" );
		int outside = 0;
		sofa::helper::vector<Octree<Vec3d>*> cells;
		for (unsigned int i=0;i<out.size();i++)
		{
			Vec3d pos = out[i];
			Vec3d coefs;
			int index = -1;
			double distance = 1e10;
			Octree<Vec3d>* cell = octree.findNear(pos);
			if (cell)
			{
				const sofa::helper::vector<int>& elems = cell->elems();
				for (unsigned int e = 0; e < elems.size(); e++)
				{
					unsigned int t = elems[e];
					Vec3d v = bases[t] * (pos - in[tetras[t][0]]);
					double d = std::max(std::max(-v[0],-v[1]),std::max(-v[2],v[0]+v[1]+v[2]-1));
					if (d>0) d = (pos-centers[t]).norm2();
					if (d<distance) { coefs = v; distance = d; index = t; }
				}
			}
			if (distance > 0)
			{ // pos is outside of the fem mesh, find the nearest tetra

				// first let's find at least one tetra that is close, if not already found
				if (index >= 0) // we already have a close tetra, we need to look only for closer ones
				{
					cells.clear();
					octree.findAllAround(cells, pos, sqrt(distance)*1.5);
					for (unsigned int ci = 0; ci < cells.size(); ++ci)
					{
						if (cells[ci] == cell) continue; // already processed this cell
						const sofa::helper::vector<int>& elems = cells[ci]->elems();
						for (unsigned int e = 0; e < elems.size(); e++)
						{
							unsigned int t = elems[e];
							double d = (pos-centers[t]).norm2();
							if (d<distance)
							{
								coefs = bases[t] * (pos - in[tetras[t][0]]);
								distance = d; index = t;
							}
						}
					}
				}
				else
				{
					// failsafe case (should not happen...), to be sure we do a brute-force search
					for (unsigned int t = 0; t < tetras.size(); t++)
					{
						double d = (pos-centers[t]).norm2();
						if (d<distance)
						{
							coefs = bases[t] * (pos - in[tetras[t][0]]);
							distance = d; index = t;
						}
					}
				}
				if (index >= 0)
				{
					//if (verbose >= 1) std::cout << "Surface vertex " << i << " mapped outside of tetra " << index << " with coefs " << coefs << std::endl;
					++outside;
				}
			}
			if (index >= 0)
			{
				//std::cout << "Surface vertex " << i << " mapped from tetra " << index << " with coefs " << coefs << std::endl;
				d_map_i[i][0] = tetras[index][0]; d_map_f[i][0] = (float)(1-coefs[0]-coefs[1]-coefs[2]);
				d_map_i[i][1] = tetras[index][1]; d_map_f[i][1] = (float)(coefs[0]);
				d_map_i[i][2] = tetras[index][2]; d_map_f[i][2] = (float)(coefs[1]);
				d_map_i[i][3] = tetras[index][3]; d_map_f[i][3] = (float)(coefs[2]);
			}
		}
		LOGI( "Mapping done: %d - vertices outside of simulation mesh: %d", outside , out.size() );
	}
}

void Rendering::Mesh::updatePositions(FEMMesh* inputMesh)
{
	const TVecCoord& in = inputMesh->positions;
	TVecCoord& out = m_vertices;
	if (d_map_f.size() != out.size() || d_map_i.size() != out.size()) return;

	DEVICE_METHOD(TetraMapper3f_apply)( out.size(), d_map_i.deviceRead(), d_map_f.deviceRead(), out.deviceWrite(), in.deviceRead() );
	const GLvoid * pointer = NULL;

	pointer = this->m_vertices.hostRead();

	glBindBuffer(GL_ARRAY_BUFFER, d_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(TCoord), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->m_vertices.size() * sizeof(TCoord), pointer );


	/*for (unsigned int i=0;i<out.size();++i)
	{
	out[i] = 
	in[map_i[i][0]] * map_f[i][0] +
	in[map_i[i][1]] * map_f[i][1] +
	in[map_i[i][2]] * map_f[i][2] +
	in[map_i[i][3]] * map_f[i][3];
	}*/
}

void Rendering::Mesh::setupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->d_VAO);

	glGenBuffers(1, &this->d_VBO);
	glGenBuffers(1, &this->d_EBO);
	glGenBuffers(1,&this->d_VBO_textures);

	glBindVertexArray(this->d_VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->d_VBO);
	// A great thing about struct is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(TCoord), &this->m_vertices[0], GL_STREAM_DRAW); 



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->d_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(GLuint), &this->m_indices[0], GL_STATIC_DRAW);



	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TCoord), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coordinates
	glEnableVertexAttribArray(2);	
	glBindBuffer(GL_ARRAY_BUFFER, this->d_VBO_textures);
	glBufferData(GL_ARRAY_BUFFER, this->m_texCoords.size() * sizeof(glm::vec2), &this->m_texCoords[0], GL_STATIC_DRAW); 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);



	glEnableVertexAttribArray(4);	
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));




	glBindVertexArray(0);
}
