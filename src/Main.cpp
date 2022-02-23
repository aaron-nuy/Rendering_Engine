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

	rtre::Cube mysphre;

	while (!window.shouldClose() && !window.isKeyPressed(GLFW_KEY_ESCAPE)) {
		rtre::setBackgroundColor(0.5, 0.1, 0.1,1.0);
		rtre::clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rtre::camera.updatePerspective();

		mysphre.draw();

		rtre::Window::pollEvents();
		window.swapBuffers();
		rtre::controller::control();
	}

	rtre::Window::terminate();
}
