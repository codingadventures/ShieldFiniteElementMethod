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
#include "FEMMesh.h"
#include "octree.h"
#include "common.h"
//#include "cpu/CPUBarycentricMapping.h"

namespace Rendering
{

	using namespace std; 

	class Mesh {
	public:
		/*  Mesh Data  */
		//vector<Vertex>				m_vertices;
		TVecCoord					m_vertices;
		vector<glm::vec2>			m_texCoords;
		vector<GLuint>				m_indices;
		vector<GLuint>				m_adjacent_indices;
		vector<Texture>				m_textures; 
		//vector<VertexWeight>		m_boneWeights;

		glm::vec3					m_center_of_mass; 
		glm::vec3					m_polyhedral_center_of_mass;

	private:						
		//BoundingBox					d_bounding_box;
		//BoundingSphere				d_bounding_sphere;

		/*  Render data  */			
		GLuint						d_VAO;
		GLuint						d_VBO;
		GLuint						d_VBO_textures;

		GLuint						d_EBO;
		GLuint						d_bone_VBO;
		//map<string, Bone>			d_bone_mapping;
		Material					d_material;
		MyVector(TTetra)			d_map_i;
		MyVector(TCoord4)			d_map_f;
		float						d_area;
	public:
		/*  Functions  */
		// Constructor
		Mesh(TVecCoord vertices, vector<GLuint> indices, vector<Texture> textures, Material material, vector<glm::vec2> textCoords);

		void init(FEMMesh* inputMesh);
		// Render the mesh
		void Draw(Shader& shader);

		float Area() const { return d_area; } 
		void updatePositions(FEMMesh* inputMesh);
 
	private:
		bool hasBones() const{
			//return m_boneWeights.size() >0;
		}
		/*  Functions    */
		// Initializes all the buffer objects/arrays
		void setupMesh();

		void updateNormals();
		// Calculation of the center of mass based on paul bourke's website
		// http://paulbourke.net/geometry/polygonmesh/
		void calculateCenterOfMass();

		void calculateArea();

		void calculateBoundingBox();

		void calculateTexCoord();
	};
	 
	//
	//void Mesh::updateNormals()
	//{
	//    normals.recreate(positions.size());
	//    if (computeTangents)
	//        tangents.recreate(positions.size());
	//#ifdef SOFA_DEVICE_CUDA
	//    if (!velems.empty())
	//    { // use GPU
	//        if (!computeTangents)
	//        {
	//            fnormals.recreate(triangles.size());
	//            CudaVisualModel3f_calcTNormals(triangles.size(), positions.size(), triangles.deviceRead(), fnormals.deviceWrite(), positions.deviceRead());
	//            CudaVisualModel3f_calcVNormals(triangles.size(), positions.size(), nbElemPerVertex, velems.deviceRead(), normals.deviceWrite(), fnormals.deviceRead(), positions.deviceRead());
	//        }
	//        else
	//        {
	//            fnormals.recreate(triangles.size());
	//            ftangents.recreate(triangles.size());
	//            CudaVisualModel3f_calcTNormalsAndTangents(triangles.size(), positions.size(), triangles.deviceRead(), fnormals.deviceWrite(), ftangents.deviceWrite(), positions.deviceRead(), texcoords.deviceRead());
	//            CudaVisualModel3f_calcVNormalsAndTangents(triangles.size(), positions.size(), nbElemPerVertex, velems.deviceRead(), normals.deviceWrite(), tangents.deviceWrite(), fnormals.deviceRead(), ftangents.deviceRead(), positions.deviceRead(), texcoords.deviceRead());
	//        }
	//    }
	//    else
	//#endif
	//    { // use CPU
	//        if (!computeTangents)
	//        {
	//            for (unsigned int i=0;i<normals.size();++i)
	//                normals[i].clear();
	//            for (unsigned int i=0;i<triangles.size();++i)
	//            {
	//                TCoord n = cross(positions[triangles[i][1]]-positions[triangles[i][0]], 
	//                                 positions[triangles[i][2]]-positions[triangles[i][0]]);
	//                n.normalize();
	//                for (unsigned int j=0;j<3;++j)
	//                    normals[triangles[i][j]] += n;
	//            }
	//            for (unsigned int i=0;i<normals.size();++i)
	//                normals[i].normalize();
	//        }
	//        else
	//        {
	//            for (unsigned int i=0;i<normals.size();++i)
	//            {
	//                normals[i].clear();
	//                tangents[i].clear();
	//            }
	//            for (unsigned int i=0;i<triangles.size();++i)
	//            {
	//                TCoord A = positions[triangles[i][0]];
	//                TCoord B = positions[triangles[i][1]];
	//                TCoord C = positions[triangles[i][2]];
	//                B -= A;
	//                C -= A;
	//                TCoord n = cross(B,C);
	//                n.normalize();
	//                TReal Au = texcoords[triangles[i][0]][0];
	//                TReal Bu = texcoords[triangles[i][1]][0];
	//                TReal Cu = texcoords[triangles[i][2]][0];
	//                Bu -= Au;
	//                Cu -= Au;
	//                TCoord t = B * Cu - C * Bu;
	//                t.normalize();
	//                for (unsigned int j=0;j<3;++j)
	//                {
	//                    normals[triangles[i][j]] += n;
	//                    tangents[triangles[i][j]] += t;
	//                }
	//            }
	//            for (unsigned int i=0;i<normals.size();++i)
	//            {
	//                tangents[i] = cross(normals[i],tangents[i]);
	//                normals[i].normalize();
	//                tangents[i].normalize();
	//            }
	//        }
	//    }
	//}


}

#endif // Mesh_h__

