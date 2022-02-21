#pragma once
#include <memory>
#include "engine_abstractions/shader.h"
#include "engine_resources/default_shaders.h"
#include "engine_rendering/camera.h"

namespace rtre {
	
	static std::shared_ptr<RenderShader> d3Shader;
	static std::shared_ptr<RenderShader> d2Shader;
	static Camera camera;
	static GLuint viewportWidth;
	static GLuint viewportHeight;


	inline void setViewport(GLuint vWidth, GLuint vHeight) {
		viewportWidth = vWidth;
		viewportHeight = vHeight;
		glViewport(0, 0, viewportWidth, viewportHeight);
	}

	/*
	Initilize glad
	Must be called after setting window context
	*/
	inline void init(GLuint viewportWidth,GLuint viewportHeight,
			const glm::vec3& pos = glm::vec3(0,0,0),GLfloat aspectRatio = 1.0f, 
			GLfloat fov = 75.0f, GLfloat zNear = 0.05f, GLfloat zFar = 500.0f) {
		gladLoadGL();
		setViewport(viewportWidth, viewportHeight);
		camera = Camera(pos, aspectRatio, fov, zNear, zFar);

		d3Shader = std::make_shared<RenderShader>(shaders::d3Vertex, shaders::d3Frag);
		d2Shader = std::make_shared<RenderShader>(shaders::d2Vertex, shaders::d2Frag);
		
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