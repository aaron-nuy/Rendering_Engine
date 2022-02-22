#pragma once
#include <iostream>
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

		const vec3 m_UpDirection = vec3(0, 1, 0);
		GLfloat nearPlane;
		GLfloat farPlane;
		GLfloat m_Fov;
		GLfloat m_AspectRatio;

		vec3 m_Position = vec3(0.f);
		vec3 m_Speed = vec3(0.f, 0.01f, 0.f);
		vec3 m_Orientation = vec3(1.f, 0.f, 0.0f);
		mat4 perspectiveM = mat4(1.f);
		mat4 viewM = mat4(1.f);
		mat4 projectionM = mat4(1.f);

	public:

		Camera operator=(Camera camera){ return camera; }

		Camera(vec3 pos = vec3(0,0,0), GLfloat aspectRatio = 1.0f,GLfloat fov = 75.0f,
			GLfloat zNear = 0.05f,GLfloat zFar = 5000.0f)
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
				* glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDirection);
		}
		inline void updatePerspective(GLfloat aspectRatio ,GLfloat fov = 75.0f,
			GLfloat zNear = 0.05f, GLfloat zFar = 500.0f) {

			m_AspectRatio = aspectRatio;
			m_Fov = fov;
			nearPlane = zNear ;
			farPlane = zFar;

			perspectiveM = glm::perspective(glm::radians(m_Fov), m_AspectRatio, nearPlane, farPlane)
				* glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDirection);

		}


		const vec3& speed() const { return m_Speed; }
		const vec3& orientation() const { return m_Orientation; }
		const vec3& position() const { return m_Position; }
		const vec3& upDirection() const { return m_UpDirection; }
		void setSpeed(const vec3& pspeed) { m_Speed = pspeed; }
		void setSpeedY(GLfloat yspeed) { m_Speed.y = yspeed; }
		void setOrientation(const vec3& porientation) { m_Orientation = glm::normalize(porientation); }
		void setPosition(const vec3& pposition) { m_Position = pposition; }
		void setPositionY(GLfloat yposition) { m_Position.y = yposition; }

		void moveForward(){ m_Position += m_Speed * m_Orientation; }
		void moveBackward(){ m_Position -= m_Speed * m_Orientation; }
		void moveRight() {m_Position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_UpDirection));}
		void moveLeft(){ m_Position -= m_Speed * glm::normalize(glm::cross(m_Orientation, m_UpDirection)); }
		void moveUp(){ m_Position += m_Speed * m_UpDirection; }
		void moveDown(){ m_Position -= m_Speed * m_UpDirection; }
		void moveForwardFPS(){ m_Position += m_Speed * glm::normalize(glm::vec3(m_Orientation.x, 0.0f, m_Orientation.z)); }
		void moveBackwardFPS(){ m_Position -= m_Speed * glm::normalize(glm::vec3(m_Orientation.x, 0.0f, m_Orientation.z)); }

		const mat4& perspective() const { return perspectiveM; }
	};
}