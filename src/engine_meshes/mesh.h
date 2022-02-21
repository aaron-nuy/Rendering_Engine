#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../engine_abstractions/sampler.h"
#include "../engine_abstractions/dtypes.h"
#include "../engine_abstractions/buffer_objects.h"


namespace rtre {

	class AbstractMesh {
	protected:
		std::vector<GLuint> m_Indices;
		std::vector<Vertex3> m_Vertices;
		std::vector<Sampler2D> m_Textures;
	
		Vao m_Vao;

		// View m_View (for camera location)

	public:

		inline std::vector<GLuint>& indices() { return m_Indices; }
		inline std::vector<Vertex3>& vertices() { return m_Vertices; }
		inline std::vector<Sampler2D>& textures() { return m_Textures; }
		
		virtual void draw(RenderShader& shader) = 0;
		virtual void draw_instanced(){}

	};

	class Mesh : public AbstractMesh{

	public:

		Mesh(const std::vector<Vertex3>& vertices, const std::vector<GLuint>& indices, const std::vector<Sampler2D>& textures)
		{
			m_Vertices = vertices;
			m_Indices = indices;
			m_Textures = textures;

			m_Vao.bind();
			Vbo vbo(vertices);
			Ebo ebo(indices);
			m_Vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex3), (void*)0);
			m_Vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex3), (void*)(3 * sizeof(float)));
			m_Vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex3), (void*)(6 * sizeof(float)));
			m_Vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex3), (void*)(9 * sizeof(float)));
			m_Vao.unbind();
			vbo.unbind();
			ebo.unbind();
		}

		virtual void draw(RenderShader& shader) override {

			shader.activate();
			m_Vao.bind();

			for (auto& texture : m_Textures) {
				Senum type = texture.type();

				texture.bind();
				if (type == rTdiffuse)
                    texture.assign(shader, "diffuse", texture.unit());
                else if (type == rTspecular)
                    texture.assign(shader, "specular", texture.unit());
                else if (type == rTnormal)
					texture.assign(shader, "normal", texture.unit());
                else
                    texture.assign(shader, "height", texture.unit());
			}

			glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		}
	};

}