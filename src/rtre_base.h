#pragma once
#include <memory>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "engine_abstractions/shader.h"
#include "engine_abstractions/sampler.h"
#include "engine_abstractions/dtypes.h"
#include "engine_resources/default_shaders.h"
#include "engine_rendering/camera.h"
#include "GLFW/rtre_Window.h"

namespace rtre {
	
	struct BasicMesh {
		std::vector<Vertex3> vertices;
		std::vector<GLuint> indices;

		BasicMesh(){}

		BasicMesh(std::vector<Vertex3>& verts, std::vector<GLuint>& inds)
			:
			vertices(verts),
			indices(inds)
		{}
	};
	BasicMesh getVertices(const std::string& path);

	static std::shared_ptr<RenderShader> d3Shader;
	static std::shared_ptr<RenderShader> skyShader;
	static std::shared_ptr<RenderShader> d2Shader;
	static std::shared_ptr<Sampler2D> texturePlaceholder;
	static std::shared_ptr<Sampler3D> cubemapPlaceholder;
	static Camera camera;
	static GLuint viewportWidth;
	static GLuint viewportHeight;
	static Window* eWindow;
	static GLfloat aspectRatio = 1;
	static BasicMesh sphereVertices;


	inline void setViewport(GLuint vWidth, GLuint vHeight) {
		viewportWidth = vWidth;
		viewportHeight = vHeight;
		aspectRatio = viewportWidth / (GLfloat)viewportHeight;
		glViewport(0, 0, viewportWidth, viewportHeight);
	}

	/*
	Initilize glad
	Must be called after setting window context
	*/
	inline void init(GLuint viewportWidth,GLuint viewportHeight, Window& window,
			const glm::vec3& pos = glm::vec3(0,0,0),GLfloat aspectRatio = 1.0f, 
			GLfloat fov = 75.0f, GLfloat zNear = 0.05f, GLfloat zFar = 500.0f) {
		if (!gladLoadGL()) {
			throw std::exception("Could not load glad.\n");
		}

		setViewport(viewportWidth, viewportHeight);

		
		texturePlaceholder = std::make_shared<Sampler2D>("engine_resources/textures/placeholder.png", 1);
		if (!texturePlaceholder) {
			throw std::exception("Could not create placeholder texture\n");
		}

		std::array<std::string, 6> cubemapPath = {
			"engine_resources/skybox_placeholder_textures/right.jpg",
			"engine_resources/skybox_placeholder_textures/left.jpg",
			"engine_resources/skybox_placeholder_textures/top.jpg",
			"engine_resources/skybox_placeholder_textures/bottom.jpg",
			"engine_resources/skybox_placeholder_textures/front.jpg",
			"engine_resources/skybox_placeholder_textures/back.jpg"
		};
		cubemapPlaceholder = std::make_shared<Sampler3D>(cubemapPath,2);
		if (!cubemapPlaceholder) {
			throw std::exception("Could not create placeholder cubemap\n");
		}


		camera = Camera(pos, aspectRatio, fov, zNear, zFar);

		eWindow = &window;
		sphereVertices = getVertices("engine_resources/sphere_mesh/globe-sphere.obj");
		d3Shader = std::make_shared<RenderShader>(shaders::d3Vertex, shaders::d3Frag);
		d2Shader = std::make_shared<RenderShader>(shaders::d2Vertex, shaders::d2Frag);
		skyShader = std::make_shared<RenderShader>(shaders::skyVertex, shaders::skyFrag);
		
		if (!d3Shader || !d2Shader || !skyShader) {
			throw std::exception("Could not load one or more shaders.\n");
		}

	}



	void enable(int glflags) {
		glEnable(glflags);
	}
	void cullFace(int glflag) {
		glCullFace(glflag);
	}

	/*
		Set which faces to be considered the front ones
		Decision is made based on the indices ordering/direction
		Arguments are either, GL_CCW or GL_CW (clockwise/counter clockwise)
	*/
	void setFrontFace(int glflag) {
		glFrontFace(glflag);
	}

	void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 0.0) {
		glClearColor(r, g, b, a);
	}
	void setBackgroundColor(vec3 vec, GLfloat a = 0.0) {
		glClearColor(vec.x, vec.y, vec.z, a);
	}
	void setBackgroundColor(vec4 vec) {
		glClearColor(vec.x, vec.y, vec.z, vec.y);
	}

	void clearBuffers(int glflags) {
		glClear(glflags);
	}


	BasicMesh processBasicMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex3> t_vertices;
		std::vector<GLuint> t_indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex3 vertex;
			vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			if (mesh->mTextureCoords[0])
			{
				vertex.txtCoord = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else {
				vertex.txtCoord = glm::vec2(0.f);
			}

			t_vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
				t_indices.push_back(face.mIndices[j]);
		}

		BasicMesh t_Mesh(t_vertices, t_indices);

		return t_Mesh;
	}
	BasicMesh getVertices(const std::string& path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw  std::exception("ERROR::ASSIMP:: Could not load basic sphere mesh.\n");
		}

		aiMesh* mesh = scene->mMeshes[scene->mRootNode->mChildren[0]->mMeshes[0]];

		return processBasicMesh(mesh, scene);
	}

}