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
		vec2 m_Position;
		vec3 m_Color;
		vec2 m_Normal;
		vec2 m_TxtCoord;

		Vertex2()
			:
			m_Position(vec2(0, 0)),
			m_Color(vec3(1, 1, 1)),
			m_Normal(vec2(0, 0)),
			m_TxtCoord(vec2(0, 0))
		{}
		Vertex2(const vec2& pos = vec2(0,0),const vec3& color = vec3(1,1,1),
					const vec2& normal = vec2(0,0), const vec2& txtcoord = vec2(0,0))
			:
			m_Position(pos),
			m_Color(color),
			m_Normal(normal),
			m_TxtCoord(txtcoord)
		{}

	};

	struct Vertex3
	{
		vec3 m_Position;
		vec3 m_Color;
		vec3 m_Normal;
		vec2 m_TxtCoord;

		Vertex3()
			:
			m_Position(vec3(0, 0, 0)),
			m_Color(vec3(1, 1, 1)),
			m_Normal(vec3(0, 0, 0)),
			m_TxtCoord(vec2(0, 0))

		{}
		Vertex3(const vec3& pos = vec3(0, 0, 0), const vec3& color = vec3(1, 1, 1),
			const vec3& normal = vec3(0, 0, 0), const vec2& txtcoord = vec2(0, 0))
			:
			m_Position(pos),
			m_Color(color),
			m_Normal(normal),
			m_TxtCoord(txtcoord)
		{}

	};
}

