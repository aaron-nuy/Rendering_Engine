#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glad/glad.h"

namespace rtre {

	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;

	class Camera {

		const vec3 upDirection = vec3(0, 1, 0);
		GLfloat nearPlane;
		GLfloat farPlane;
		GLfloat m_Fov;
		GLfloat m_AspectRatio;

		vec3 m_Position;
		vec3 m_Speed = vec3(0, 0.5f, 0);
		vec3 m_Orientation = vec3(0, 1, 0);
		mat4 perspectiveM = mat4(1);
		mat4 viewM = mat4(1);
		mat4 projectionM = mat4(1);

	public:

		Camera operator=(Camera camera){ return camera; }

		Camera(vec3 pos = vec3(0,0,0), GLfloat aspectRatio = 1.0f,GLfloat fov = 75.0f,
			GLfloat zNear = 0.05f,GLfloat zFar = 500.0f)
			:
			m_Position(pos),
			m_Fov(fov),
			m_AspectRatio(aspectRatio),
			nearPlane(zNear),
			farPlane(zFar)	
		{
		}

		inline void updatePerspective() 
		{
			perspectiveM = glm::perspective(glm::radians(m_Fov), m_AspectRatio, nearPlane, farPlane) 
				* glm::lookAt(m_Position, m_Position + m_Orientation, upDirection);
		}
		inline void updatePerspective(GLfloat aspectRatio ,GLfloat fov = 75.0f,
			GLfloat zNear = 0.05f, GLfloat zFar = 500.0f) {

			m_AspectRatio = aspectRatio;
			m_Fov = fov;
			nearPlane = zNear ;
			farPlane = zFar;

			perspectiveM = glm::perspective(glm::radians(m_Fov), m_AspectRatio, nearPlane, farPlane)
				* glm::lookAt(m_Position, m_Position + m_Orientation, upDirection);

		}


		const vec3& speed() const { return m_Speed; }
		const vec3& orientation() const { return m_Orientation; }
		const vec3& position() const { return m_Position; }
		void setSpeed(const vec3& pspeed) { m_Speed = pspeed; }
		void setSpeedY(GLfloat yspeed) { m_Speed.y = yspeed; }
		void setOrientation(const vec3& porientation) { m_Orientation = glm::normalize(porientation); }
		void setPosition(const vec3& pposition) { m_Position = pposition; }
		void setPositionY(GLfloat yposition) { m_Position.y = yposition; }

		const mat4& perspective() const { return perspectiveM; }
	};
}