#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace rtre {

	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;
	struct Vertex2
	{
		vec2 position;
		vec3 color;
		vec2 normal;
		vec2 txtCoord;

		Vertex2()
			:
			position(vec2(0, 0)),
			color(vec3(1, 1, 1)),
			normal(vec2(0, 0)),
			txtCoord(vec2(0, 0))
		{}
		Vertex2(const vec2& pos ,const vec3& color = vec3(1,1,1),
					const vec2& normal = vec2(0,0), const vec2& txtcoord = vec2(0,0))
			:
			position(pos),
			color(color),
			normal(normal),
			txtCoord(txtcoord)
		{}

	};

	struct Vertex3
	{
		vec3 position;
		vec3 color;
		vec3 normal;
		vec2 txtCoord;

		Vertex3() 
			:
			position(vec3(0, 0, 0)),
			color(vec3(1, 1, 1)),
			normal(vec3(0, 0, 0)),
			txtCoord(vec2(0, 0))

		{}
		Vertex3(const vec3& pos, const vec3& color = vec3(1, 1, 1),
			const vec3& normal = vec3(0, 0, 0), const vec2& txtcoord = vec2(0, 0))
			:
			position(pos),
			color(color),
			normal(normal),
			txtCoord(txtcoord)
		{}

	};
}

