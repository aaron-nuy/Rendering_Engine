#pragma once
#include "Ebo.h"
#include "Vao.h"
#include "Sampler.h"

namespace rtre {

	/*
		Initilize glad
		Must be called after setting window context
	*/
	void init() {
		gladLoadGL();
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
		glClearColor = vec4(vec.x, vec.y, vec.z, a);
	}
	void setBackgroundColor(vec4 vec) {
		glClearColor = vec4(vec.x, vec.y, vec.z, vec.y);
	}

	void clearBuffers(int glflags) {
		glClear(glflags);
	}
}