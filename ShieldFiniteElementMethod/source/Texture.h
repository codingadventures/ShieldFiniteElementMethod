#ifndef Texture_h__
#define Texture_h__

#include <string>

enum TextureType{ 
	TextureType_DIFFUSE,
	TextureType_SPECULAR,
	TextureType_NORMAL,
	TextureType_REFLECTION
};

struct Texture {
	unsigned int id;
	TextureType m_texture_type;
	std::string m_file_name;
	std::string m_directory;
	std::string m_uniform_name;
	bool  m_has3dTexture;
	Texture(std::string file_name, TextureType type, std::string uniform_name = "");
	Texture(std::string directory, std::string uniform_name = "");
	bool Load(std::string directory);
	std::string Get_Uniform_Name(std::string index);
};



 
#endif // Texture_h__