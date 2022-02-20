#pragma once
#include <vector>
#include <array>
#include "glad/glad.h"
#include "glm/glm.hpp"


namespace rtre {
	
	class Ebo {

		GLuint m_ID = 0;

	public:
		Ebo() {
			glGenBuffers(1, &m_ID);
		}
		Ebo(Ebo& eb) {
			m_ID = eb.id();
		}
		Ebo(Ebo&& eb) noexcept {
			m_ID = eb.id();
			eb.free();
		}

		~Ebo() {
			glDeleteBuffers(1, &m_ID);
		}


		Ebo(const std::vector<GLuint>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
		}
		template<std::size_t n>
		Ebo(const std::array<GLuint, n>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
		}
		Ebo(GLuint* vertices, GLsizeiptr size) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}


		inline void loadData(const std::vector<GLuint>& vertices) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}
		template<std::size_t n>
		inline void loadData(const std::array<GLuint,n>& vertices) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}
		inline void loadData(GLuint* vertices, GLsizeiptr size) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}		

		inline void bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		}
		inline void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}
		inline void free() {
			glDeleteBuffers(1, &m_ID);
		}

		inline GLuint id() {
			return m_ID;
		}
	};


}