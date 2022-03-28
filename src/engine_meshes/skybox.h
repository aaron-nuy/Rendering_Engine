#pragma once
#include <vector>
#include "mesh.h"
#include "../engine_abstractions/shader.h"
#include "../rtre_base.h"

namespace rtre {
	class Skybox {
		std::vector<std::shared_ptr<Sampler3D>> m_CubeMaps;
		std::shared_ptr<RenderShader> m_Shader;
		vec4 m_Scale = vec4(1);

		static const std::vector<Vertex3> m_Vertices;
		static const std::vector<GLuint> m_Indices;

		Vao m_Vao;
		Vbo m_Vbo;
		Ebo m_Ebo;

		void loadSkybox() {
			m_Vao.bind();
			m_Ebo.loadData(m_Indices);
			m_Vbo.loadData(m_Vertices);
			m_Vao.linkAttrib(0, 3, GL_FLOAT, sizeof(Vertex3), (void*)0);
			m_Vao.unbind();
			m_Ebo.unbind();
			m_Vbo.unbind();
		}


	public:
		
		Skybox(){
			m_Shader = skyShader;
			m_CubeMaps.push_back(cubemapPlaceholder);
			loadSkybox();
		}
		Skybox(std::shared_ptr<RenderShader> shader) {
			m_Shader = shader;
			m_CubeMaps.push_back(cubemapPlaceholder);
			loadSkybox();
		}
		Skybox(const std::array<std::string,6>& textures) {
			m_Shader = skyShader;
			m_CubeMaps.push_back(std::make_shared<Sampler3D>(textures,2));
			loadSkybox();
		}
		Skybox(const std::array<std::string, 6>& textures, std::shared_ptr<RenderShader> shader) {

			m_CubeMaps.push_back(std::make_shared<Sampler3D>(textures,2));
			m_Shader = shader;
			loadSkybox();
		}
		Skybox(const std::array<std::string, 6>& textures, const char* vertexShaderPath, const char* fragShaderPath,
			const char* geometryShaderPath = nullptr) {
			m_CubeMaps.push_back(std::make_shared<Sampler3D>(textures,2));
			m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
			loadSkybox();
		}
		Skybox(const char* vertexShaderPath, const char* fragShaderPath,
			const char* geometryShaderPath = nullptr){
			m_Shader = skyShader;
			m_Shader = std::make_shared<RenderShader>(vertexShaderPath, fragShaderPath, geometryShaderPath);
			loadSkybox();
		}


		void draw() {
			m_Shader->activate();
			m_Vao.bind();

			glDepthFunc(GL_LEQUAL);

			for (auto& map : m_CubeMaps) {
				Senum type = map->type();

				map->bind();
				if (type == rTdiffuse)
					map->assign(*m_Shader, "diffuse", map->unit());
				else if (type == rTspecular)
					map->assign(*m_Shader, "specular", map->unit());
				else if (type == rTnormal)
					map->assign(*m_Shader, "normal", map->unit());
				else
					map->assign(*m_Shader, "height", map->unit());
		
			}

			m_Shader->SetUniform(m_Shader->getUnifromID("perspectiveM"), camera.skyboxPers());
			m_Shader->SetUniform(m_Shader->getUnifromID("scaleV"), m_Scale);
			m_Shader->SetUniform(m_Shader->getUnifromID("aspectRatioV"), aspectRatio);

			glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

			glDepthFunc(GL_LESS);

		}

	};

	const std::vector<Vertex3> Skybox::m_Vertices = {
		Vertex3(vec3(-1.0f, -1.0f,  1.0f),vec2(0)),
		Vertex3(vec3( 1.0f, -1.0f,  1.0f),vec2(0)),
		Vertex3(vec3( 1.0f, -1.0f, -1.0f),vec2(0)),
		Vertex3(vec3(-1.0f, -1.0f, -1.0f),vec2(0)),
		Vertex3(vec3(-1.0f,  1.0f,  1.0f),vec2(0)),
		Vertex3(vec3( 1.0f,  1.0f,  1.0f),vec2(0)),
		Vertex3(vec3( 1.0f,  1.0f, -1.0f),vec2(0)),
		Vertex3(vec3(-1.0f,  1.0f, -1.0f),vec2(0))
	};

	const std::vector<GLuint> Skybox::m_Indices = {
		// Right
		6, 2, 1,
		1, 5, 6,
		// Left
		7, 4, 0,
		0, 3, 7,
		// Top
		6, 5, 4,
		4, 7, 6,
		// Bottom
		2, 3, 0,
		0, 1, 2,
		// Back
		5, 1, 0,
		0, 4, 5,
		// Front
		6, 7, 3,
		3, 2, 6
	};
}