#ifndef Material_h__
#define Material_h__

#include "glm\detail\type_vec3.hpp"
#include "Shader.h"
namespace Rendering
{
	using namespace Shaders;
	class Material
	{
	public:
		Material(){}
		Material(glm::vec3  ambient, glm::vec3  diffuse,glm::vec3  specular,float shininess); 
		void SetShader(Shader& shader); 

	private:

		glm::vec3	d_ambient;
		glm::vec3	d_diffuse;
		glm::vec3	d_specular;

		float		d_shininess;
	};
}
#endif // Material_h__
