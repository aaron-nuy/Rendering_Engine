#pragma once
#include <vector>
#include <array>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Vertex2.h"
#include "Vertex3.h"

namespace rtre 
{
	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;

	class Vbo {
		GLuint m_ID = 0;

	public:

		Vbo() {
			glGenBuffers(1, &m_ID);
		}
		Vbo(Vbo& vb) {
			m_ID = vb.id();
		}
		Vbo(Vbo&& vb) noexcept{
			m_ID = vb.id();
			vb.free();
		}
		template<class T>
		Vbo(const std::vector<T>& vertices)  {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		}
		template<class T,std::size_t n>
		Vbo(const std::array<T, n>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		}
		Vbo(GLfloat* vertices, GLsizeiptr size) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size , vertices, GL_STATIC_DRAW);
		}

		~Vbo() {
			glDeleteBuffers(1,&m_ID);
		}
		
		template<class T>
		inline void loadData(const std::vector<T>& vertices) {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}
		template<class T,std::size_t n>
		inline void loadData(const std::array<T,n>& vertices) {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}
		inline void loadData(GLfloat* vertices, GLsizeiptr size) {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}

		inline void bind() {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}
		inline void unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}
		inline void free() { 
			glDeleteBuffers(1, &m_ID);
		}

		inline GLuint id() const {
			return m_ID;
		}

	};

}