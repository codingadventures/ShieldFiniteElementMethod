#include "Texture.h"


Texture::Texture(string& file_name, TextureType type, string uniform_name ) 
	: m_texture_type(type),m_file_name(file_name), m_uniform_name(uniform_name), m_has3dTexture(false)
{
	assert(file_name!= "");
}

Texture::Texture(string& directory, string uniform_name /*= ""*/) 
	: m_directory(directory), m_uniform_name(uniform_name), m_has3dTexture(false)
{

}

std::string Texture::Get_Uniform_Name(string index)
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

bool Texture::Load(string& directory)
{
	 FILE *file;

	 string filename(m_file_name);
	 string fullPath = directory + "\\" + filename;

	if ((file = fopen(filename.c_str(), "rb")) == NULL)
	{
		std::cerr << "File not found : " << filename << std::endl;
		return 0;
	}
	
	png_structp PNG_reader = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (PNG_reader == NULL)
	{
		std::cerr << "png_create_read_struct failed for file "<< filename << std::endl;
		fclose(file);
		return 0;
	}
	
	png_infop PNG_info = png_create_info_struct(PNG_reader);
	png_infop PNG_end_info = png_create_info_struct(PNG_reader);
	if (PNG_info == NULL || PNG_end_info == NULL)
	{
		std::cerr << "png_create_info_struct failed for file " << filename << std::endl;
		png_destroy_read_struct(&PNG_reader, NULL, NULL);
		fclose(file);
		return 0;
	}
	
	if (setjmp(png_jmpbuf(PNG_reader)))
	{
		std::cerr << "Loading failed for PNG file " << filename << std::endl;
		png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
		fclose(file);
		return 0;
	}
	
	png_init_io(PNG_reader, file);
	
	png_read_info(PNG_reader, PNG_info);
	
	png_uint_32 width, height;
	width = png_get_image_width(PNG_reader, PNG_info);
	height = png_get_image_height(PNG_reader, PNG_info);
	
	png_uint_32 bit_depth, channels, color_type;
	bit_depth = png_get_bit_depth(PNG_reader, PNG_info);
	channels = png_get_channels(PNG_reader, PNG_info);
	color_type = png_get_color_type(PNG_reader, PNG_info);
	
	std::cout << "PNG image "<<filename<<": "<<width<<"x"<<height<<"x"<<bit_depth*channels<<std::endl;
	bool changed = false;
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(PNG_reader);
		changed = true;
	}
	
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
#if PNG_LIBPNG_VER >= 10209
		png_set_expand_gray_1_2_4_to_8(PNG_reader);
#else
        png_set_gray_1_2_4_to_8(PNG_reader);    // deprecated from libpng 1.2.9
#endif
		changed = true;
	}
	/*
	if (bit_depth == 16)
	{
		png_set_strip_16(PNG_reader);
		changed = true;
	}
    */
	if (changed)
	{
		png_read_update_info(PNG_reader, PNG_info);
		bit_depth = png_get_bit_depth(PNG_reader, PNG_info);
		channels = png_get_channels(PNG_reader, PNG_info);
		color_type = png_get_color_type(PNG_reader, PNG_info);
		std::cout << "Converted PNG image "<<filename<<": "<<width<<"x"<<height<<"x"<<bit_depth*channels<<std::endl;
	}

    GLint internalFormat; // 1, 2, 3, or 4, or one of the following symbolic constants: GL_ALPHA, GL_ALPHA4, GL_ALPHA8, GL_ALPHA12, GL_ALPHA16, GL_COMPRESSED_ALPHA, GL_COMPRESSED_LUMINANCE, GL_COMPRESSED_LUMINANCE_ALPHA, GL_COMPRESSED_INTENSITY, GL_COMPRESSED_RGB, GL_COMPRESSED_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32, GL_LUMINANCE, GL_LUMINANCE4, GL_LUMINANCE8, GL_LUMINANCE12, GL_LUMINANCE16, GL_LUMINANCE_ALPHA, GL_LUMINANCE4_ALPHA4, GL_LUMINANCE6_ALPHA2, GL_LUMINANCE8_ALPHA8, GL_LUMINANCE12_ALPHA4, GL_LUMINANCE12_ALPHA12, GL_LUMINANCE16_ALPHA16, GL_INTENSITY, GL_INTENSITY4, GL_INTENSITY8, GL_INTENSITY12, GL_INTENSITY16, GL_R3_G3_B2, GL_RGB, GL_RGB4, GL_RGB5, GL_RGB8, GL_RGB10, GL_RGB12, GL_RGB16, GL_RGBA, GL_RGBA2, GL_RGBA4, GL_RGB5_A1, GL_RGBA8, GL_RGB10_A2, GL_RGBA12, GL_RGBA16, GL_SLUMINANCE, GL_SLUMINANCE8, GL_SLUMINANCE_ALPHA, GL_SLUMINANCE8_ALPHA8, GL_SRGB, GL_SRGB8, GL_SRGB_ALPHA, or GL_SRGB8_ALPHA8.
    GLenum format; // GL_COLOR_INDEX, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA.
    GLenum type; // GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV.
    switch (bit_depth)
    {
    case 8:
        type = GL_UNSIGNED_BYTE;
        break;
    case 16:
        type = GL_UNSIGNED_SHORT;
        break;
    default:
        std::cerr << "PNG: in " << filename << ", unsupported bit depth: " << bit_depth << std::endl;
        return 0;
    }
    

    unsigned int lineSize = width * ((channels*bit_depth+7)/8);
    // align to 4 bytes
    lineSize = (lineSize + 3) & -4;
    png_byte* data = (png_byte*)malloc(height*lineSize);
	png_byte** PNG_rows = (png_byte**)malloc(height * sizeof(png_byte*));
	for (png_uint_32 row = 0; row < height; ++row)
        PNG_rows[height - 1 - row] = data+row*lineSize;
	
	png_read_image(PNG_reader, PNG_rows);
	
	free(PNG_rows);
	
	png_read_end(PNG_reader, PNG_end_info);
	
	png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
	fclose(file);
	 

	//SOIL_load_OGL_texture(filename.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB|SOIL_FLAG_COMPRESS_TO_DXT);;
	glGenTextures(1, &id);
	//int width,height,channels;
	//unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, nullptr, 0);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,  0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	free(data);
	// Parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST); 
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glGenerateMipmap(GL_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D, 0);

	
//	delete image; 
	//	SOIL_free_image_data(image); 
	return true;
}