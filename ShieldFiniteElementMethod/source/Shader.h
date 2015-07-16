#ifndef Shader_h__
#define Shader_h__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream> 

#include "common.h"

namespace Shaders
{
	using namespace std;

	class Shader
	{
	public:
		GLuint m_program;

	private:
		vector<string> d_vertex_source_path;
		vector<string> d_fragment_source_path;
		vector<string> d_geometry_source_path;

		GLchar** d_vertex_code;

		GLchar** d_fragment_code;

		GLchar** d_geometry_code;

		GLint* d_vertex_string_count;
		GLint* d_fragment_string_count;
		GLint* d_geometry_string_count;

		const size_t d_n_vertex;
		const size_t d_n_fragment;
		const size_t d_n_geometry;

		GLint d_mvp_uniform;
		GLint d_vertex_shader;
		GLint d_fragment_shader;
		GLint d_geometry_shader;
	public:
		// Constructor reads and builds our shader
		Shader(string vertexSourcePath, string fragmentSourcePath, string geometrySourcePath = "");

		Shader(vector<string> vertex_source_paths, string fragmentSourcePath, string geometrySourcePath = "");

		Shader(vector<string> vertex_source_paths, vector<string> fragment_source_paths, string geometrySourcePath = "");

		~Shader();

		// Use the shader program
		void Use();

		void SetUniform(string const &name, glm::vec3 const &value);

		void SetUniform(string const &name, glm::vec4 const &value);

		void SetUniform(string const &name, glm::mat4 const &value);

		void SetUniform(string const &name, float value);

		void SetUniform(string const &name, bool value);
	private:

		void init();

		void loadVertex();

		void loadFragment();

		void loadGeometry();

		static void load(string sourcePath, GLchar*& output, GLint& count);

		void compile();

		void link();

		void initPointers();

		GLint getUniformLocation(string const &name);
	};
}
#endif // Shader_h__