
#include "Texture.h"
#include "common.h"

#include "SOIL.h"


Texture::Texture(std::string file_name, TextureType type, std::string uniform_name ) 
	: m_texture_type(type),m_file_name(file_name), m_uniform_name(uniform_name), m_has3dTexture(false)
{

}

Texture::Texture(std::string directory, std::string uniform_name /*= ""*/) 
	: m_directory(directory), m_uniform_name(uniform_name), m_has3dTexture(false)
{

}

std::string Texture::Get_Uniform_Name(std::string index)
{
	if (!m_uniform_name.empty()) return m_uniform_name;

	switch (m_texture_type)
	{
	case TextureType_REFLECTION:
		return "material.texture_reflection" + index;
		break;

	case TextureType_DIFFUSE:
		return "material.texture_diffuse" + index;
		break;
	case TextureType_SPECULAR:
		return "material.texture_specular" + index;
		break;
	case TextureType_NORMAL:
		return "material.texture_normal" + index;
		break;

	default:
		break;
	}
}

bool Texture::Load(std::string directory)
{  
	int width, height, channels;
	unsigned char *ht_map = SOIL_load_image
		(m_file_name.c_str()
		,
		&width, &height, &channels,
		SOIL_LOAD_RGB
		);


	glGenTextures(1, &id);
	//int width,height,channels;
	//unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, nullptr, 0);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, id);
	// Parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); /* */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	/*glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glGenerateMipmap(GL_TEXTURE_2D); */
	GLuint format;
	switch (channels)
	{
	case 3:
		format = GL_RGB; 
		break;
	case 4:
		format = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D,  0, GL_RGB , width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ht_map );

	SOIL_free_image_data( ht_map );

	//	delete image; 
	//	SOIL_free_image_data(image); 
	return true;
}