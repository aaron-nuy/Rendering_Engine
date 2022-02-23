#pragma once
#include <string>

namespace rtre { namespace shaders {

	static const char* skyVertex = 
		"#version 430 core\n"

		"layout (location = 0) in vec3 aPos; \n"

		"out vec4 TxtCoords;\n"

		"uniform mat4 perspectiveM;\n"
		"uniform vec4 scaleV;\n"
		"uniform float aspectRatioV;\n"


		"void main()\n{\n"
		"TxtCoords = vec4(aPos.x,aPos.y,-aPos.z,1.0f);\n"
		"vec4 pos = perspectiveM*vec4(aPos,1.0f);\n"
		"gl_Position =  scaleV*vec4(pos.x,pos.y,pos.w,pos.w); \n"
		"}\n\0";;

	static const char* skyFrag = 
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec4 TxtCoords;\n"

		"uniform samplerCube diffuse;\n"
		"uniform samplerCube specular;\n"
		"uniform samplerCube normal;\n"
		"uniform samplerCube height;\n"

		"void main()\n{\n"
		"FragColor = texture(diffuse,TxtCoords.xyz);\n"
		"}\n\0";

	static const char* d3Vertex =
		"#version 430 core\n"

		"layout (location = 0) in vec3 aPos; \n"
		"layout (location = 1) in vec2 aTxtCoords;\n"

		"out vec2 TxtCoords;\n"

		"uniform mat4 perspectiveM;\n"
		"uniform mat4 rotateM;\n"
		"uniform vec4 scaleV;\n"
		"uniform float aspectRatioV;\n"
		"uniform vec3 positionV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"TxtCoords = aTxtCoords;\n"
		"gl_Position =  perspectiveM*(rotateM*(vec4(positionV,0)+scaleV*vec4(aPos,1.0f))); \n"
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

		"uniform mat4 rotateM;\n"
		"uniform vec4 scaleV;\n"
		"uniform vec2 positionV;\n"
		"uniform float aspectRatioV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"TxtCoords = aTxtCoords;\n"

		"gl_Position = (rotateM*(vec4(positionV,0.f,0.f)+scaleV*vec4(aPos,0.0f,1.0f))) * scaleView(aspectRatioV); \n"
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