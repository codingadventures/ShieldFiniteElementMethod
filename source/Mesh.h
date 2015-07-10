#ifndef Mesh_h__
#define Mesh_h__


// Std. Includes
#include <sstream>
#include <vector>
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h" 
// GL Includes 
#include "Material.h"
#include "glm/gtc/constants.hpp"
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp> 
#include <glm/detail/type_vec3.hpp>

namespace Rendering
{

	using namespace std; 

	class Mesh {
	public:
		/*  Mesh Data  */
		vector<Vertex>				m_vertices;
		vector<GLuint>				m_indices;
		vector<GLuint>				m_adjacent_indices;
		vector<Texture>				m_textures; 
		vector<VertexWeight>		m_boneWeights;

		glm::vec3					m_center_of_mass; 
		glm::vec3					m_polyhedral_center_of_mass;

	private:						

		/*  Render data  */			
		GLuint						d_VAO;
		GLuint						d_VBO;
		GLuint						d_EBO;
		GLuint						d_bone_VBO; 
		Material					d_material;

		float						d_area;
	public:
		/*  Functions  */
		// Constructor
		Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, vector<VertexWeight> boneWeights, vector<GLuint> adjacent_indices, Material material);


		// Render the mesh
		void Draw( Shader& shader, bool withAdjecencies = false)  ;

		float Area() const { return d_area; } 



	private:
		bool hasBones() const{
			return m_boneWeights.size() >0;
		}
		/*  Functions    */
		// Initializes all the buffer objects/arrays
		void setupMesh();

		// Calculation of the center of mass based on paul bourke's website
		// http://paulbourke.net/geometry/polygonmesh/
		void calculateCenterOfMass();

		void calculateArea();

		void calculateBoundingBox();

		void calculateTexCoord();
	};

	inline Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, vector<VertexWeight> boneWeights, vector<GLuint> adjacent_indices, Material material): 
	m_adjacent_indices(adjacent_indices), 
		d_material(material),
		d_area(0.0f)
	{ 
		this->m_vertices = vertices;
		this->m_indices = indices;
		this->m_textures = textures; 
		this->m_boneWeights = boneWeights; 
		//this->calculate_center_of_mass();
		this->calculateArea();
		this->calculateBoundingBox(); 
		//this->calculate_tex_coord();
		this->setupMesh();

	}

	inline void Mesh::Draw( Shader&   shader, bool withAdjecencies)  
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

		GLuint drawMode =   GL_TRIANGLES;
		GLuint indices_size = withAdjecencies ? m_adjacent_indices.size() : m_indices.size();
		if (withAdjecencies)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->d_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_adjacent_indices.size() * sizeof(GLuint), &this->m_adjacent_indices[0], GL_STATIC_DRAW);

		}
		glDrawElements(drawMode, indices_size, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);
	}

	inline void Mesh::setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->d_VAO);
		glGenBuffers(1, &this->d_VBO);
		glGenBuffers(1, &this->d_EBO);

		glBindVertexArray(this->d_VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->d_VBO);
		// A great thing about struct is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(Vertex), &this->m_vertices[0], GL_STATIC_DRAW);  

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->d_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(GLuint), &this->m_indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);	
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coordinates
		glEnableVertexAttribArray(2);	
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));


		/*glEnableVertexAttribArray(3);	
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));*/

		//
		glEnableVertexAttribArray(4);	
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));

		if (hasBones())
		{
			glGenBuffers(1, &this->d_bone_VBO);


			glBindBuffer(GL_ARRAY_BUFFER, d_bone_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_boneWeights[0]) * m_boneWeights.size(), &m_boneWeights[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(5);
			glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexWeight), (const GLvoid*)0);

			glEnableVertexAttribArray(6);    
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWeight), (const GLvoid*)offsetof(VertexWeight, Weights)); 
		}



		glBindVertexArray(0);
	}

	inline void Mesh::calculateCenterOfMass()
	{
	}

	inline void Mesh::calculateArea()
	{	 
		size_t N = m_vertices.size();
		if (N % 3 != 0) return;

		for (int i = 0; i < N; i = i + 3)
		{
			glm::vec3 v1 = m_vertices[i].Position;
			glm::vec3 v2 = m_vertices[i+1].Position;
			glm::vec3 v3 = m_vertices[i+2].Position;
			d_area += glm::length(glm::cross(v2 - v1,v3 - v1)) * 0.5f;
		}
	}



	inline void Mesh::calculateTexCoord()
	{
		size_t N = m_vertices.size();
		if (N % 3 != 0) return;

		for (int i = 0; i < N; i = i + 3)
		{
			auto v1 = m_vertices[i].Position;
			auto v2 = m_vertices[i+1].Position;
			auto v3 = m_vertices[i+2].Position;

			auto mod1 = fmod( atan2( v1.z, v1.x )  +  glm::pi<float>() , glm::pi<float>());
			auto mod2 = fmod( atan2( v2.z, v2.x )  +  glm::pi<float>() , glm::pi<float>());
			auto mod3 = fmod( atan2( v3.z, v3.x )  +  glm::pi<float>() , glm::pi<float>());

			m_vertices[i].TexCoords		= glm::vec2(1.0f - ( mod1 / glm::pi<float>() * 0.5f ), 0.5f - v1.y / 2.0f );
			m_vertices[i+1].TexCoords	= glm::vec2( 1.0f - ( mod2  / glm::pi<float>() * 0.5 ), 0.5 - v2.y / 2.0f );
			m_vertices[i+2]	.TexCoords	=  glm::vec2( 1.0f - ( mod3 / glm::pi<float>() * 0.5 ), 0.5 - v3.y / 2.0f );
		}
	}
}

#endif // Mesh_h__

