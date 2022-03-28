#pragma once
#include <memory>
#include <iostream>
#include <algorithm>
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
	
	static std::shared_ptr<RenderShader> d3Shader;
	static std::shared_ptr<RenderShader> skyShader;
	static std::shared_ptr<RenderShader> d2Shader;
	static std::shared_ptr<RenderShader> d2CircleShader;
	static std::shared_ptr<Sampler2D> texturePlaceholder;
	static std::shared_ptr<Sampler3D> cubemapPlaceholder;
	static Camera camera;
	static GLuint viewportWidth;
	static GLuint viewportHeight;
	static Window* eWindow;
	static GLfloat aspectRatio = 1;
	static BasicMesh sphereVertices;
	static std::vector<PointLight*> pointLights;

	void sendLights(std::shared_ptr<RenderShader> shader) {
		shader->SetUniform(shader->getUnifromID("nPointLights"), (GLuint)pointLights.size());

		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->passStruct(shader, i);
		}
	}

	void setViewport(GLuint vWidth, GLuint vHeight) {
		viewportWidth = vWidth;
		viewportHeight = vHeight;
		aspectRatio = viewportWidth / (GLfloat)viewportHeight;

		camera.setAspectRatio(aspectRatio);

		glViewport(0, 0, viewportWidth, viewportHeight);
	}

	/*
	Initilize glad
	Must be called after setting window context
	*/
	void init(GLuint viewportWidth,GLuint viewportHeight, Window& window,
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
		sphereVertices = BasicMesh::getVertices("engine_resources/sphere_mesh/globe-sphere.obj");
		d3Shader = std::make_shared<RenderShader>(shaders::d3Vertex, shaders::d3Frag);
		d2Shader = std::make_shared<RenderShader>(shaders::d2Vertex, shaders::d2Frag);
		d2CircleShader = std::make_shared<RenderShader>(shaders::d2Vertex, shaders::d2CircleFrag);
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
	/*
	Set which faces to be considered the front ones
	Decision is made based on the indices ordering/direction
	Arguments are either, GL_CCW or GL_CW (clockwise/counter clockwise)
	*/
	void setFrontFace(int glflag) {
		glFrontFace(glflag);
	}


	// Member functions are defined here to gain access to pointLights vector
	PointLight::PointLight(vec3 pos, vec3 dif, vec3 spec,
		GLfloat cons, GLfloat lin, GLfloat quad)
		:
		position(pos),
		diffuse(dif),
		specular(spec),
		constant(cons),
		linear(lin),
		quadratic(quad)

	{
		pointLights.push_back(this);
	}
	PointLight::~PointLight() {
		pointLights.erase(std::remove(pointLights.begin(), pointLights.end(), this), pointLights.end());
	}

}