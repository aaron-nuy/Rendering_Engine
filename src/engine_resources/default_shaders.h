#pragma once
#include <string>

namespace rtre { namespace shaders {

	static const char* d3Vertex =
		"#version 430 core\n"

		"layout (location = 0) in vec3 aPos; \n"
		"layout (location = 1) in vec2 aTxtCoords;\n"

		"out vec2 TxtCoords;\n"

		"uniform mat4 perspectiveM;\n"
		"uniform mat4 transformM;\n"
		"uniform mat4 rotateM;\n"
		"uniform float scaleV;\n"
		"uniform float aspectRatioV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"TxtCoords = aTxtCoords;\n"
		"gl_Position =  perspectiveM*(rotateM*vec4(scaleV*aPos,1.0f)*transformM); \n"
		"}\n\0";

	static const char* d3Frag =
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec2 TxtCoords;\n"

		"uniform sampler2D diffuse;\n"
		"uniform sampler2D specular;\n"
		"uniform sampler2D normal;\n"
		"uniform sampler2D height;\n"

		"void main()\n{\n"
		"FragColor = texture(diffuse,TxtCoords);\n"
		"}\n\0";

	static const char* d2Vertex =
		"#version 430 core\n"

		"layout (location = 0) in vec2 aPos; \n"
		"layout (location = 1) in vec2 aTxtCoords;\n"

		"out vec2 TxtCoords;\n"

		"uniform mat4 transformM;\n"
		"uniform mat4 rotateM;\n"
		"uniform float scaleV;\n"
		"uniform float aspectRatioV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"TxtCoords = aTxtCoords;\n"

		"gl_Position = transformM*(scaleV * (rotateM*vec4(aPos,0.0f,1.0f)) * scaleView(aspectRatioV)); \n"
		"}\n\0";

	static const char* d2Frag =
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec2 TxtCoords;\n"

		"uniform sampler2D diffuse;\n"

		"void main()\n{\n"
		"FragColor = texture(diffuse,TxtCoords);\n"
		"}\n\0";


	
}}