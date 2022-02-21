#pragma once
#include <string>

namespace rtre { namespace shaders {

	static const char* d3Vertex =
		"#version 430 core\n"

		"layout (location = 0) in vec3 aPos; \n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec3 aNormal;\n"
		"layout (location = 3) in vec2 aTxtCoords;\n"

		"out vec2 TxtCoords;\n"
		"out vec3 Color;\n"
		"out vec3 Normal;\n"

		"uniform mat4 perspectiveM;\n"
		"uniform mat4 transformM;\n"
		"uniform mat4 rotateM;\n"
		"uniform float scaleV;\n"
		"uniform float aspectRationV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"TxtCoords = aTxtCoords;\n"
		"Color = aColor;\n"
		"Normal = aNormal;\n"
		"gl_Position =  scaleV* (rotateM*vec4(aPos,1.0f)) *scaleView(aspectRationV)* transformM * perspectiveM; \n"
		"}\n\0";

	static const char* d3Frag =
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec2 TxtCoords;\n"
		"in vec3 Color;\n"
		"in vec3 Normal;\n"

		"uniform sampler2D diffuse;\n"
		"uniform sampler2D specular;\n"
		"uniform sampler2D normal;\n"
		"uniform sampler2D height;\n"

		"void main()\n{\n"
		"FragColor = vec4(Color,0.0) + texture(diffuse,TxtCoords);\n"
		"}\n\0";

	static const char* d2Vertex =
		"#version 430 core\n"

		"layout (location = 0) in vec2 aPos; \n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 3) in vec2 aTxtCoords;\n"

		"out vec2 TxtCoords;\n"
		"out vec3 Color;\n"

		"uniform mat4 transformM;\n"
		"uniform mat4 rotateM;\n"
		"uniform float scaleV;\n"
		"uniform float aspectRationV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"TxtCoords = aTxtCoords;\n"
		"Color = aColor;\n"
		"gl_Position = scaleV * (rotateM*vec4(aPos,0.0f,0.0f)) * scaleView(aspectRationV)  * transformM; \n"
		"}\n\0";

	static const char* d2Frag =
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec2 TxtCoords;\n"
		"in vec2 Normal;\n"
		"in vec3 Color;\n"

		"uniform sampler2D diffuse;\n"

		"void main()\n{\n"
		"FragColor = vec4(Color,0.0f) + texture(diffuse,TxtCoords);\n"
		"}\n\0";



}}