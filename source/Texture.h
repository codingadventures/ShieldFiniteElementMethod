#ifndef Texture_h__
#define Texture_h__

 
#include "common.h"
#include "png.h"

using namespace std;

enum TextureType{ 
	TextureType_DIFFUSE,
	TextureType_SPECULAR,
	TextureType_NORMAL,
	TextureType_REFLECTION
};

struct Texture {
	GLuint id;
	TextureType m_texture_type;
	string m_file_name;
	string m_directory;
	string m_uniform_name;
	bool  m_has3dTexture;
	Texture(string& file_name, TextureType type, string uniform_name = "");
	Texture(string& directory, string uniform_name = "");
	bool Load(string& directory);
	bool Load3D(vector<string> textures);
	string Get_Uniform_Name(string index);
};


 

#endif // Texture_h__