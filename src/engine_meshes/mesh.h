#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../engine_abstractions/sampler.h"
#include "../engine_abstractions/dtypes.h"
#include "../engine_abstractions/buffer_objects.h"


namespace rtre {

	class Mesh{
		std::vector<GLuint> m_Indices;
		std::vector<Vertex3> m_Vertices;
		std::vector<std::shared_ptr<Sampler2D>> m_Textures;
		Vao m_Vao;
		Vbo m_Vbo;
		Ebo m_Ebo;
	public:

		Mesh(){}

		Mesh(const std::vector<Vertex3>& vertices, const std::vector<GLuint>& indices, 
			const std::vector<std::shared_ptr<Sampler2D>>& textures)
		{
			m_Vertices = vertices;
			m_Indices = indices;
			m_Textures = textures;

			m_Vao.bind();
			m_Vbo.loadData(vertices);
			m_Ebo.loadData(indices);
			m_Vao.linkAttrib(0, 3, GL_FLOAT, sizeof(Vertex3), (void*)0);
			m_Vao.linkAttrib(1, 2, GL_FLOAT, sizeof(Vertex3), (void*)(sizeof(glm::vec3)));
			m_Vao.linkAttrib(2, 3, GL_FLOAT, sizeof(Vertex3), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
			m_Vao.unbind();
			m_Vbo.unbind();
			m_Ebo.unbind();
		}

		void load(const std::vector<Vertex3>& vertices, const std::vector<GLuint>& indices,
			const std::vector<std::shared_ptr<Sampler2D>>& textures) {
			m_Vertices = vertices;
			m_Indices = indices;
			m_Textures = textures;

			m_Vao.bind();
			m_Vbo.loadData(vertices);
			m_Ebo.loadData(indices);
			m_Vao.linkAttrib(0, 3, GL_FLOAT, sizeof(Vertex3), (void*)0);
			m_Vao.linkAttrib(1, 2, GL_FLOAT, sizeof(Vertex3), (void*)(sizeof(glm::vec3)));
			m_Vao.linkAttrib(2, 3, GL_FLOAT, sizeof(Vertex3), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
			m_Vao.unbind();
			m_Vbo.unbind();
			m_Ebo.unbind();
		}

		inline std::vector<GLuint>& indices() { return m_Indices; }
		inline std::vector<Vertex3>& vertices() { return m_Vertices; }
		inline std::vector<std::shared_ptr<Sampler2D>>& textures() { return m_Textures; }


		inline void draw(std::shared_ptr<RenderShader>& shader)  {

			m_Vao.bind();

			for (auto& texture : m_Textures) {
				Senum type = texture->type();

				texture->bind();
				if (type == rTdiffuse)
                    texture->assign(*shader, "diffuse", texture->unit());
                else if (type == rTspecular)
                    texture->assign(*shader, "specular", texture->unit());
                else if (type == rTnormal)
					texture->assign(*shader, "normal", texture->unit());
                else
                    texture->assign(*shader, "height", texture->unit());
			}

			glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

		}
	};

}