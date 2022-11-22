#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Bout {
	class ShaderProgram;
	
	class ShaderStage {
		friend class ShaderProgram;
	public:
		ShaderStage(const char* fname, unsigned int id) :
			m_filename{ fname},
			m_id{ id }
		{

			auto f = std::ifstream(m_filename);
			auto s = std::stringstream{};

			try {
				s << f.rdbuf();
				f.close();
			}
			catch (std::ifstream::failure e) {
				std::cout << e.what() << std::endl;
				return;
			}

			auto CodeString = s.str();
			if (CodeString.size() == 0) {
				std::cout << "Failed to load shader code for " << m_filename << '\n';
				return;
			}
			compile(CodeString);
		};
		virtual ~ShaderStage() {
			if (m_id)
				glDeleteShader(m_id);
		}

	//protected:
		const std::string m_filename;
		unsigned int m_id;

	private:
		void compile(const std::string &code) {
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
			}
		}
		
	};

	class VertexShader : public ShaderStage {
	public:
		VertexShader(const char* fname) :
			ShaderStage(fname, glCreateShader(GL_VERTEX_SHADER)) {}
	};

	class FragmentShader : public ShaderStage {
	public:
		FragmentShader(const char* fname) :
			ShaderStage(fname, glCreateShader(GL_FRAGMENT_SHADER)) {}
	};

	class GeometryShader : public ShaderStage {
	public:
		GeometryShader(const char* fname) :
			ShaderStage(fname, glCreateShader(GL_GEOMETRY_SHADER)) {}
	};

	class ShaderProgram {
	public:

		ShaderProgram() : m_id{ 0 } {}

		ShaderProgram(VertexShader vs, FragmentShader fs) :
			m_id{ glCreateProgram() }
		{
			glAttachShader(m_id, vs.m_id);
			glAttachShader(m_id, fs.m_id);
			glLinkProgram(m_id);
		}

		ShaderProgram(VertexShader vs, FragmentShader fs, GeometryShader gs) :
			m_id{ glCreateProgram() }
		{
			glAttachShader(m_id, vs.m_id);
			glAttachShader(m_id, fs.m_id);
			glAttachShader(m_id, gs.m_id);
			glLinkProgram(m_id);
		}

		ShaderProgram(const ShaderProgram& rhs)
		{
			this->m_id = rhs.m_id; 
		}

		void SetFloat(const char* name, float value)
		{
			glUniform1f(glGetUniformLocation(this->m_id, name), value);
		}

		void SetInteger(const char* name, int value)
		{
			glUniform1i(glGetUniformLocation(this->m_id, name), value);
		}

		void SetVector2f(const char* name, float x, float y)
		{
			glUniform2f(glGetUniformLocation(this->m_id, name), x, y);
		}

		void SetVector2f(const char* name, const glm::vec2& value)
		{
			glUniform2f(glGetUniformLocation(this->m_id, name), value.x, value.y);
		}

		void SetVector3f(const char* name, float x, float y, float z)
		{
			glUniform3f(glGetUniformLocation(this->m_id, name), x, y, z);
		}

		void SetVector3f(const char* name, const glm::vec3& value)
		{
			glUniform3f(glGetUniformLocation(this->m_id, name), value.x, value.y, value.z);
		}

		void SetVector4f(const char* name, float x, float y, float z, float w)
		{
			glUniform4f(glGetUniformLocation(this->m_id, name), x, y, z, w);
		}

		void SetVector4f(const char* name, const glm::vec4& value)
		{
			glUniform4f(glGetUniformLocation(this->m_id, name), value.x, value.y, value.z, value.w);
		}

		void SetMatrix4(const char* name, const glm::mat4& matrix)
		{
			glUniformMatrix4fv(glGetUniformLocation(this->m_id, name), 1, false, glm::value_ptr(matrix));
		}

		void use()
		{
			glUseProgram(this->m_id);
		}
	//private:
		unsigned int m_id;
	};


}
