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
		"layout (location = 2) in vec3 aNormal;\n"

		"out vec3 vPosition;\n"
		"out vec2 TxtCoords;\n"
		"out vec3 Normal;\n"

		"uniform mat4 perspectiveM;\n"
		"uniform mat4 rotateM;\n"
		"uniform vec4 scaleV;\n"
		"uniform float aspectRatioV;\n"
		"uniform vec3 positionV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"vPosition = aPos;\n"
		"Normal = normalize(aNormal);\n"
		"TxtCoords = aTxtCoords;\n"
		"gl_Position =  perspectiveM*(rotateM*(vec4(positionV,0)+scaleV*vec4(aPos,1.0f))); \n"
		"}\n\0";

	static const char* d3Frag =
		"#version 430 core\n"
		"#define MAX_LIGHTS 128\n"

		"struct PointLight {\n"
			"vec3 position;\n"
		
			"vec3 diffuse;\n"
			"vec3 specular;\n"
		
			"float constant;\n"
			"float linear;\n"
			"float quadratic;\n"
		"};\n"

		"out vec4 FragColor;\n"

		"in vec3 vPosition;\n"
		"in vec3 Normal;\n"
		"in vec2 TxtCoords;\n"

		"uniform sampler2D diffuse;\n"
		"uniform sampler2D specular;\n"
		"uniform sampler2D normal;\n"
		"uniform sampler2D height;\n"
		"uniform unsigned int nPointLights;\n"
		"uniform PointLight pointLights[MAX_LIGHTS];\n"


		"void main()\n{\n"

		"vec4 totallight;"
		"float ambient = 0.1f;"
		"for(unsigned int i=0;i<nPointLights;i++){\n"
		"	vec3 lightDirection = pointLights[i].position - vPosition;\n"
		"	float Distance = length(lightDirection); "
		"	Distance = 0.4*pow(Distance,2)+ 0.13f*Distance + 1.00f;"
		"	vec3 lightVector = normalize(lightDirection);"
		"	float diffuseModifier = max(dot(Normal, lightVector), 0.0f);"
		"	totallight = totallight + vec4((pointLights[i].diffuse * diffuseModifier)*0.5/Distance,1.0f);"
		"}\n"

		"FragColor = (totallight+vec4(ambient))*texture(diffuse,TxtCoords);\n"
		"}\n\0";

	static const char* d2Vertex =
		"#version 430 core\n"

		"layout (location = 0) in vec2 aPos; \n"
		"layout (location = 1) in vec2 aTxtCoords;\n"

		"out vec2 vPosition;\n"
		"out vec2 TxtCoords;\n"

		"uniform mat4 rotateM;\n"
		"uniform vec2 positionV;\n"
		"uniform vec2 scaleV;\n"
		"uniform float aspectRatioV;\n"

		"vec4 scaleView(float ratio){\n"
		"return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )"
		"       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));}\n"

		"void main()\n{\n"
		"vPosition = aPos;\n"
		"TxtCoords = aTxtCoords;\n"

		"gl_Position = (rotateM*(vec4(positionV,0.f,0.f)+vec4(scaleV,1,1)*vec4(aPos,-1.0f,1.0f))) * scaleView(aspectRatioV); \n"
		"}\n\0";

	static const char* d2Frag =
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec2 vPosition;\n"
		"in vec2 TxtCoords;\n"

		"uniform sampler2D diffuse;\n"

		"void main()\n{\n"
		"FragColor = texture(diffuse,TxtCoords);\n"
		"}\n\0";

	static const char* d2CircleFrag =
		"#version 430 core\n"

		"out vec4 FragColor;\n"

		"in vec2 vPosition;\n"
		"in vec2 TxtCoords;\n"

		"uniform sampler2D diffuse;\n"

		"void main()\n{\n"

		"if(length(vPosition)>0.5f) discard;\n"
		"FragColor = texture(diffuse,TxtCoords);\n"
		"}\n\0";



	
}}