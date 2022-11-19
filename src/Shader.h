#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>


namespace ogl {
	class ShaderProgram;
	
	class Shader {
		friend class ShaderProgram;
	public:
		Shader(const char* fname, unsigned int id) :
			m_filename{ fname},
			m_id{ id }
		{
			auto f = std::ifstream("C:\\Projects\\BreakoutGL\\shaders\\" + m_filename);
			auto s = std::stringstream{};

			try {
				s << f.rdbuf();
				f.close();
			}
			catch (std::ifstream::failure e) {
				std::cout << e.what() << std::endl;
				return;
			}

			compile(s.str());
		};
		virtual ~Shader() {
			if (m_id)
				glDeleteShader(m_id);
		}

	//protected:
		const std::string m_filename;
		unsigned int m_id;

	private:
		bool compile(const std::string& code) {
			auto code_cstr = code.c_str();
			glShaderSource(m_id, 1, &code_cstr, NULL);
			glCompileShader(m_id);

			int  success;
			char infoLog[512];
			glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(m_id, 512, NULL, infoLog);
				std::cout << m_filename << ": ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
				return false;
			}
			return true;
		}
		
	};

	class VertexShader : public Shader{
	public:
		VertexShader(const char* fname) :
			Shader(fname, glCreateShader(GL_VERTEX_SHADER)) {}
	};

	class FragmentShader : public Shader {
	public:
		FragmentShader(const char* fname) :
			Shader(fname, glCreateShader(GL_FRAGMENT_SHADER)) {}
	};

	class ShaderProgram {
	public:
		ShaderProgram(VertexShader vs, FragmentShader fs) :
			m_id{ glCreateProgram() },
			m_vs{ vs },
			m_fs{ fs }
		{
			glAttachShader(m_id, vs.m_id);
			glAttachShader(m_id, fs.m_id);
			glLinkProgram(m_id);
		}

		void use() { glUseProgram(this->m_id); }
	//private:
		unsigned int m_id;
		VertexShader m_vs;
		FragmentShader m_fs;

	};


}
