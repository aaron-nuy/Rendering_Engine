#pragma once
#include "Vbo.h"

namespace rtre {
	class Vao {
		GLuint m_ID = 0;

	public:

		Vao() {
			glGenVertexArrays(1, &m_ID);
		}

		~Vao() {
			glDeleteVertexArrays(1, &m_ID);
		}


		/*
			Links a VBO to the current Vao
			
			NOTE : This function does not bind the Vao, it must be binded before calling it
		*/
		inline void linkAttrib(Vbo& pvbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {

			pvbo.bind();
			glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
			glEnableVertexAttribArray(layout);
			pvbo.unbind();

		}

		inline void bind() {
			glBindVertexArray(m_ID);
		}
		inline void unbind() {
			glBindVertexArray(0);
		}
		inline void free() {
			glDeleteVertexArrays(1, &m_ID);
		}

		inline GLuint id() {
			return m_ID;
		}
	};
}