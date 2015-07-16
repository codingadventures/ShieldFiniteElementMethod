#ifndef Model_h__
#define Model_h__


// Std. Includes
#include <string>
#include <iostream>
#include <map>
#include <vector> 
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include "Mesh.h" 

namespace Rendering
{

	using namespace std; 

	class Model 
	{
	public:

		glm::mat4*		m_animation_matrix;
		//Skeleton*		m_skeleton;
		glm::vec3		m_Direction;

	private:  
		vector<Mesh>	d_meshes;

		vector<Texture> d_textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

		string			d_directory;

		GLuint*			d_bone_location;
		int				d_numberOfBone;

		glm::mat4		d_model_matrix;
		glm::mat4		d_scale;
		glm::mat4		d_position;


		glm::quat		d_rotation;


	public:
		/*  Functions   */
		// Constructor, expects a filepath to a 3D model.
		explicit Model(GLchar* path); 

		vector<Mesh>* GetMeshes();

		glm::vec3		Position() const; 

		bool			Has_Texture();

		~Model();

		vector<Mesh>* Meshes();
		void addTextures(Texture texture);
		void Draw(Shader& shader );
		void Scale(glm::vec3 const& scale_vector);
		void Translate(glm::vec3 const& translation_vector);
		void TranslateFromOrigin(glm::vec3 const& translation_vector);
		void Rotate(glm::vec3 const& rotation_vector, float radians);
		void Rotate(glm::quat rotation);
		glm::quat Rotation() const;
		glm::vec3 GetPositionVec();
		glm::mat4 GetPosition();
		glm::mat4 GetModelMatrix() const;


	private:
		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		/*  Model Data  */



		Mesh processMesh(aiMesh* ai_mesh, const aiScene* scene );
		/*  Functions   */
		// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);


	};

}
#endif // Model_h__