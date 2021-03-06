#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace rtre {

	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;

	std::string get_file_contents(const char* filename) {
		std::ifstream file(filename, std::ios::binary);
		if (file)
		{
			std::string contents;
			file.seekg(0, std::ios::end);
			contents.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&contents[0], contents.size());
			file.close();
			return(contents);
		}
		throw(std::exception("Couldn't load shader file.\n"));
	}

	class AbstractShader {

	protected:
		GLuint m_ID = 0;

	public:
		inline void activate() {
			glUseProgram(m_ID);
		}
		inline virtual void free() {
			glDeleteProgram(m_ID);
		}

		static inline void checkError(GLuint shaderID, const char* type) {

			GLint hasCompiled;

			char infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shaderID, GL_COMPILE_STATUS, &hasCompiled);
				if (hasCompiled == GL_FALSE)
				{
					glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
					std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shaderID, GL_LINK_STATUS, &hasCompiled);
				if (hasCompiled == GL_FALSE)
				{
					glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
					std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
				}
			}
		}

		inline GLint getUnifromID(const char* name) const {
			return glGetUniformLocation(m_ID, name);
		}

		inline void SetUniform(GLint uniID, mat4 value) {
			glUniformMatrix4fv(uniID, 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetUniform(GLint uniID, mat3 value) {
			glUniformMatrix3fv(uniID, 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetUniform(GLint uniID, mat2 value) {
			glUniformMatrix2fv(uniID, 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetUniform(GLint uniID, GLfloat v1, GLfloat v2) {
			glUniform2f(uniID, v1, v2);
		}
		inline void SetUniform(GLint uniID, vec2 vec) {
			glUniform2f(uniID, vec.x, vec.y);
		}
		inline void SetUniform(GLint uniID, GLfloat v1, GLfloat v2, GLfloat v3) {
			glUniform3f(uniID, v1, v2, v3);
		}
		inline void SetUniform(GLint uniID, vec3 vec) {
			glUniform3f(uniID, vec.x, vec.y, vec.z);
		}
		inline void SetUniform(GLint uniID, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) {
			glUniform4f(uniID, v1, v2, v3, v4);
		}
		inline void SetUniform(GLint uniID, vec4 vec) {
			glUniform4f(uniID, vec.x, vec.y, vec.z, vec.w);
		}
		inline void SetUniform(GLint uniID, GLint value) {
			glUniform1i(uniID, value);
		}
		inline void SetUniform(GLint uniID, GLuint value) {
			glUniform1ui(uniID, value);
		}
		inline void SetUniform(GLint uniID, GLfloat value) {
			glUniform1f(uniID, value);
		}

		inline GLuint id() const {
			return m_ID;
		}

	};


	class RenderShader : public AbstractShader {

	public:

		RenderShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = NULL)
		{
			 const char* vertexCode;
			 const char* fragmentCode;

			try {
				std::string vertexSource = get_file_contents(vertexFile);
				std::string fragSource = get_file_contents(fragmentFile);
				// Read vertexFile and fragmentFile and store the strings
				vertexCode = vertexSource.c_str();
				fragmentCode = fragSource.c_str();
			}
			catch (std::exception& e) {
				vertexCode = vertexFile;
				fragmentCode = fragmentFile;
			}

			// Create Vertex Shader Object and get its reference
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexCode, NULL);
			glCompileShader(vertexShader);
			checkError(vertexShader, "VERTEX");

			// Create Fragment Shader Object and get its reference
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
			glCompileShader(fragmentShader);
			checkError(fragmentShader, "FRAGMENT");

			// Create Shader Program Object and get its reference
			m_ID = glCreateProgram();
			glAttachShader(m_ID, vertexShader);
			glAttachShader(m_ID, fragmentShader);
			glLinkProgram(m_ID);
			checkError(m_ID, "PROGRAM");

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

		}

		~RenderShader() {
			glDeleteProgram(m_ID);
		}
	};
}