#pragma once
#include <memory>
#include "engine_abstractions/shader.h"
#include "engine_abstractions/sampler.h"
#include "engine_resources/default_shaders.h"
#include "engine_rendering/camera.h"
#include "GLFW/ar_WGL.h"

namespace rtre {
	

	static std::shared_ptr<RenderShader> d3Shader;
	static std::shared_ptr<RenderShader> skyShader;
	static std::shared_ptr<RenderShader> d2Shader;
	static std::shared_ptr<Sampler2D> texturePlaceholder;
	static std::shared_ptr<Sampler3D> cubemapPlaceholder;
	static Camera camera;
	static GLuint viewportWidth;
	static GLuint viewportHeight;
	static WGLWindow* eWindow;
	static GLfloat aspectRatio = 1;


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
	inline void init(GLuint viewportWidth,GLuint viewportHeight, WGLWindow* window,
			const glm::vec3& pos = glm::vec3(0,0,0),GLfloat aspectRatio = 1.0f, 
			GLfloat fov = 75.0f, GLfloat zNear = 0.05f, GLfloat zFar = 500.0f) {
		gladLoadGL();

		setViewport(viewportWidth, viewportHeight);

		texturePlaceholder = std::make_shared<Sampler2D>("engine_resources/textures/placeholder.png",1);

		std::array<std::string, 6> cubemapPath = {
			"engine_resources/skybox_placeholder_textures/right.jpg",
			"engine_resources/skybox_placeholder_textures/left.jpg",
			"engine_resources/skybox_placeholder_textures/top.jpg",
			"engine_resources/skybox_placeholder_textures/bottom.jpg",
			"engine_resources/skybox_placeholder_textures/front.jpg",
			"engine_resources/skybox_placeholder_textures/back.jpg"
		};
		cubemapPlaceholder = std::make_shared<Sampler3D>(cubemapPath,2);

		camera = Camera(pos, aspectRatio, fov, zNear, zFar);

		eWindow = window;

		d3Shader = std::make_shared<RenderShader>(shaders::d3Vertex, shaders::d3Frag);
		d2Shader = std::make_shared<RenderShader>(shaders::d2Vertex, shaders::d2Frag);
		skyShader = std::make_shared<RenderShader>(shaders::skyVertex, shaders::skyFrag);
		
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
}