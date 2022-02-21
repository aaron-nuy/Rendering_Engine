#pragma once
#include <memory>
#include "engine_abstractions/shader.h"
#include "engine_resources/default_shaders.h"

namespace rtre {
	
	std::shared_ptr<RenderShader> d3Shader;
	std::shared_ptr<RenderShader> d2Shader;

	/*
		Initilize glad
		Must be called after setting window context
	*/
	void init() {
		gladLoadGL();
		
		d3Shader = std::make_shared<RenderShader>(shaders::d3Vertex, shaders::d3Frag);
		d2Shader = std::make_shared<RenderShader>(shaders::d2Vertex, shaders::d2Frag);
		
	}

	void setViewport(GLuint width, GLuint height) {
		glViewport(0, 0, width, height);
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