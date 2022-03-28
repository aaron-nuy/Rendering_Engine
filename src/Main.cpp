#include <vector>
#include "rtre.h"
#include "GLFW/rtre_Window.h"
#include "engine_movement/controller.h"

int main()
{
	rtre::Window::init();
	rtre::Window::initHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	rtre::Window::initHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	rtre::Window::initHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	rtre::Window window(1000, 1000,"Engine");
	window.makeContextCurrent();


	rtre::init(1000, 1000,window);
	rtre::enable(GL_DEPTH_TEST);
	rtre::enable(GL_CULL_FACE);
	rtre::enable(GL_FRONT);
	rtre::setFrontFace(GL_CCW);

	rtre::Skybox myskybox;

	rtre::Sphere m;

	rtre::PointLight mylight(glm::vec3(3, 1, 1),glm::vec3(1,1,1));

	while (!window.shouldClose() && !window.isKeyPressed(GLFW_KEY_ESCAPE)) {
		rtre::clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rtre::setBackgroundColor(0.5, 0.1, 0.1, 1.0);
		rtre::camera.updatePerspective();

		mylight.position = glm::rotateY(mylight.position,glm::radians(.05f));

		m.draw();
		

		myskybox.draw();

		rtre::controller::control();
		rtre::Window::pollEvents();
		window.swapBuffers();
	}

	rtre::Window::terminate();
}
