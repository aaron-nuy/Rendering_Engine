#pragma once
#include "../engine_abstractions/rtre_engine.h"

namespace rtre {

	class AbstractMesh {
	protected:
		std::vector<GLuint> m_Indices;
		std::vector<Vertex3> m_Vertices;
		std::vector<Sampler> m_Textures;
	
		Vao m_Vao;

		// View m_View (for camera location)

	public:

		inline std::vector<GLuint>& indices() { return m_Indices; }
		inline std::vector<Vertex3>& vertices() { return m_Vertices; }
		inline std::vector<Sampler>& textures() { return m_Textures; }
		
		virtual void draw() = 0;
		virtual void draw_instanced(){}


	};



	class Mesh : public AbstractMesh{

		RenderShader* m_Shader;

	public:

		Mesh(const std::vector<Vertex3>& vertices, const std::vector<GLuint>& indices, const std::vector<Sampler>& textures) 
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

		virtual void draw() override {

			m_Shader->activate();
			m_Vao.bind();

			for (auto& texture : m_Textures) {
				Senum type = texture.type();

				texture.bind();
				if (type == diffuse)
					texture.assign(*m_Shader, "diffuse", texture.unit());
				else if (type == specular)
					texture.assign(*m_Shader, "specular", texture.unit());
				else
					texture.assign(*m_Shader, "metalic", texture.unit());
			}

			glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

		}

	};

	class Model {

		std::vector<Mesh> meshes;
		RenderShader* m_Shader;

	public:

	};

}