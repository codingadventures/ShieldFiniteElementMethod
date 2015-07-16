
#include "Texture.h"
 

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
	//Magick::Blob blob;
	//Magick::Image* image; 
	//string stringFileName(m_file_name);
	//string fullPath = directory + "\\" + stringFileName;
	//try {
	//	image = new Magick::Image(fullPath);
	//	image->write(&blob, "RGBA");
	//}
	//catch (Magick::Error& Error) {
	//	std::cout << "Error loading texture '" << fullPath << "': " << Error.what() << std::endl;
	//	return false;
	//}


	//int col = image->columns();

	////SOIL_load_OGL_texture(filename.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB|SOIL_FLAG_COMPRESS_TO_DXT);;
	//glGenTextures(1, &id);
	////int width,height,channels;
	////unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, nullptr, 0);
	//// Assign texture to ID
	//glBindTexture(GL_TEXTURE_2D, id);
	//glTexImage2D(GL_TEXTURE_2D,  0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());

	//// Parameters
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );  
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST); 

	//glGenerateMipmap(GL_TEXTURE_2D);	
	//glBindTexture(GL_TEXTURE_2D, 0);


	//	delete image; 
	//	SOIL_free_image_data(image); 
	return true;
}