#include "Shader.h"

namespace Shaders
{
	 Shader::Shader(string vertexSourcePath, string fragmentSourcePath, string geometrySourcePath): d_n_vertex(1),d_n_fragment(1), d_n_geometry(geometrySourcePath.empty() ? 0 : 1)
	{
		d_vertex_source_path.push_back(vertexSourcePath);
		d_fragment_source_path.push_back(fragmentSourcePath);

		if (!geometrySourcePath.empty())
			d_geometry_source_path.push_back(geometrySourcePath);

		init();
	}

	 Shader::Shader(vector<string> vertex_source_paths, string fragmentSourcePath, string geometrySourcePath):
	d_vertex_source_path(vertex_source_paths),
		d_n_vertex(vertex_source_paths.size()),
		d_n_fragment(1),
		d_n_geometry(geometrySourcePath.empty() ? 0 : 1)
	{
		d_fragment_source_path.push_back(fragmentSourcePath);

		if (!geometrySourcePath.empty())
			d_geometry_source_path.push_back(geometrySourcePath);


		init();
	}

	 Shader::Shader(vector<string> vertex_source_paths, vector<string> fragment_source_paths, string geometrySourcePath):
	d_vertex_source_path(vertex_source_paths),
		d_fragment_source_path(fragment_source_paths),

		d_n_vertex(vertex_source_paths.size()),
		d_n_fragment(fragment_source_paths.size()) ,
		d_n_geometry(geometrySourcePath.empty() ? 0 : 1)
	{
		if (!geometrySourcePath.empty())
			d_geometry_source_path.push_back(geometrySourcePath);

		init();
	}

	 Shader::~Shader()
	{
		for (auto i = 0; i < d_n_vertex; i++)
		{
			delete[] d_vertex_code[i];
		}
		delete[] d_vertex_code;
		delete d_vertex_string_count;

		if (d_n_geometry != 0)
		{
			for (auto i = 0; i < d_n_fragment; i++)
			{
				delete[] d_fragment_code[i];
			}
			delete[] d_geometry_code;
			delete d_geometry_string_count;
		}


		for (auto i = 0; i < d_n_geometry; i++)
		{
			delete[] d_geometry_code[i];
		}


		delete[] d_fragment_code;
		delete d_fragment_string_count;
	}

	 void Shader::Use()
	{
		glUseProgram(this->m_program);
	}

	 void Shader::SetUniform(string const &name, glm::vec3 const &value)
	{
		auto iUniform = getUniformLocation(name);
		glUniform3fv(iUniform, 1, glm::value_ptr(value));
	}

	 void Shader::SetUniform(string const &name, glm::vec4 const &value)
	{
		auto iUniform = getUniformLocation(name);
		glUniform4fv(iUniform, 1, glm::value_ptr(value));
	}

	 void Shader::SetUniform(string const &name, glm::mat4 const &value)
	{
		auto iUniform = getUniformLocation(name);
		glUniformMatrix4fv(iUniform, 1, GL_FALSE, glm::value_ptr(value));
	}

	 void Shader::SetUniform(string const &name, float value)
	{
		auto iUniform = getUniformLocation(name);
		glUniform1f(iUniform, value);
	}

	 void Shader::SetUniform(string const &name, bool value)
	{ 
		auto iUniform = getUniformLocation(name);
		glUniform1i(iUniform, value);
	}

	 void Shader::init()
	{
		initPointers();
		loadVertex();

		if (d_n_geometry != 0)
			loadGeometry();

		loadFragment();
		compile();
		link();
	}

	 void Shader::loadVertex()
	{
		for (size_t i = 0; i < d_n_vertex; i++)
		{
			load(d_vertex_source_path[i], d_vertex_code[i], d_vertex_string_count[i]);
		}
	}

	 void Shader::loadFragment()
	{
		for (size_t i = 0; i < d_n_fragment; i++)
		{
			load(d_fragment_source_path[i], d_fragment_code[i], d_fragment_string_count[i]);
		}
	}

	 void Shader::loadGeometry()
	{
		for (size_t i = 0; i < d_n_geometry; i++)
		{
			load(d_geometry_source_path[i], d_geometry_code[i], d_geometry_string_count[i]);
		}
	}

	 void Shader::load(string source_path, GLchar*& output, GLint& count)
	{
		string return_code;
		try
		{
			// Open files
			ifstream vShaderFile(source_path);
			//ifstream fShaderFile(d_fragment_source_path);
			stringstream vShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			//fShaderStream << fShaderFile.rdbuf();		
			// close file handlers
			vShaderFile.close();
			//fShaderFile.close();
			// Convert stream into GLchar array
			return_code = vShaderStream.str();
			//	d_fragment_code = fShaderStream.str();		
		}
		catch (exception)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << source_path << endl;
		}

		count = return_code.length() ;
		output = new GLchar[count + 1];
		auto length = return_code.copy(output, count, 0);
		output[length] = '\0';
	}

	 void Shader::compile()
	{
		GLint success;
		auto logSize = 0;
		GLchar* infoLog;


		d_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		d_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

#pragma region [ Vertex Shader ]
		glShaderSource(d_vertex_shader, d_n_vertex, d_vertex_code, d_vertex_string_count);
		glCompileShader(d_vertex_shader);

		glGetShaderiv(d_vertex_shader, GL_COMPILE_STATUS, &success);
		if (success != 1)
		{
			glGetShaderiv(d_vertex_shader, GL_INFO_LOG_LENGTH, &logSize);
			infoLog = new GLchar[logSize];

			glGetShaderInfoLog(d_vertex_shader, logSize, nullptr, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
			delete infoLog;
		}
#pragma endregion
//
//#pragma region [ Geometry Shader ]
//		if (d_n_geometry != 0)
//		{
//			d_geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
//
//			glShaderSource(d_geometry_shader, d_n_geometry, d_geometry_code, d_geometry_string_count);
//			glCompileShader(d_geometry_shader);
//			glGetShaderiv(d_geometry_shader, GL_INFO_LOG_LENGTH, &success);
//			if (success != 1)
//			{
//				glGetShaderiv(d_geometry_shader, GL_INFO_LOG_LENGTH, &logSize);
//
//				infoLog = new GLchar[logSize];
//
//				glGetShaderInfoLog(d_geometry_shader, logSize, nullptr, infoLog);
//				cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << endl;
//				delete infoLog;
//			}
//		}
//#pragma endregion

#pragma region [ Fragment Shader ]
		glShaderSource(d_fragment_shader, d_n_fragment, d_fragment_code, d_fragment_string_count);
		glCompileShader(d_fragment_shader);
		glGetShaderiv(d_fragment_shader, GL_INFO_LOG_LENGTH, &success);
		if (success != 1)
		{
			glGetShaderiv(d_fragment_shader, GL_INFO_LOG_LENGTH, &logSize);

			infoLog = new GLchar[logSize];

			glGetShaderInfoLog(d_fragment_shader, logSize, nullptr, infoLog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
			delete infoLog;
		}
#pragma endregion


		this->m_program = glCreateProgram();
		glAttachShader(this->m_program, d_vertex_shader);

		if (d_n_geometry != 0)
			glAttachShader(this->m_program, d_geometry_shader);

		glAttachShader(this->m_program, d_fragment_shader);
	}

	 void Shader::link()
	{
		GLint success;
		GLchar* infoLog;

		glLinkProgram(this->m_program);

		glGetProgramiv(this->m_program, GL_LINK_STATUS, &success);

		if (!success)
		{
			auto logSize = 0;
			glGetProgramiv(this->m_program, GL_INFO_LOG_LENGTH, &logSize);
			infoLog = new GLchar[logSize];
			glGetProgramInfoLog(this->m_program, logSize, nullptr, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n";
			cout << "File names: \n";
			for (auto vertex : d_vertex_source_path)
			{
				cout << vertex + "\n";
			}

			for (auto fragment : d_fragment_source_path)
			{
				cout << fragment + "\n";
			}

			cout << infoLog << endl;
			delete infoLog;
		}

		glDeleteShader(d_vertex_shader);
		glDeleteShader(d_fragment_shader);
		if (d_n_geometry != 0)
			glDeleteShader(d_geometry_shader);
	}

	 void Shader::initPointers()
	{
		d_vertex_code = new GLchar*[d_n_vertex];
		d_vertex_string_count = new GLint[d_n_vertex];

		d_fragment_code = new GLchar*[d_n_fragment];
		d_fragment_string_count = new GLint[d_n_fragment];

		d_geometry_code = new GLchar*[d_n_geometry];
		d_geometry_string_count = new GLint[d_n_geometry];
	}

	 GLint Shader::getUniformLocation(string const &name)
	 {
		 return glGetUniformLocation(m_program, name.c_str());
	 }

}