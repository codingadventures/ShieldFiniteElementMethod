#include "Material.h"

namespace Rendering{

	Material::Material(glm::vec3 ambient, glm::vec3 diffuse,glm::vec3 specular,float shininess)
		:	
		d_ambient (ambient) ,
		d_diffuse (diffuse) ,
		d_specular (specular),
		d_shininess( shininess)

	{

	}


	void Material::SetShader(Shader& shader)
	{
		shader.SetUniform("material.ambient",d_ambient);
		shader.SetUniform("material.diffuse",d_diffuse);
		shader.SetUniform("material.specular",d_specular);
		shader.SetUniform("material.shininess",d_shininess);
	}

}