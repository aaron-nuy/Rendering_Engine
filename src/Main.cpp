#include <vector>
#include "rtre.h"
#include "GLFW/ar_WGL.h"
#include "engine_movement/controller.h"

std::string to_string(glm::vec3 vec) {
	return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z);
}

int main()
{


	WGLWindow::init();
	WGLWindow::initHint(GLFW_CONTEXT_VERSION_MAJOR,4 );
	WGLWindow::initHint(GLFW_CONTEXT_VERSION_MINOR,3 );
	WGLWindow::initHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	WGLWindow window(1000, 1000,"Hey you");
	window.makeContextCurrent();



	rtre::init(1000, 1000,&window);
	rtre::enable(GL_DEPTH_TEST);
	rtre::enable(GL_CULL_FACE);
	rtre::enable(GL_FRONT);
	rtre::setFrontFace(GL_CCW);

	rtre::Cube my;


	while (!window.shouldClose() && !window.isKeyPressed(GLFW_KEY_ESCAPE)) {
		rtre::setBackgroundColor(0.5, 0.1, 0.1,1.0);
		rtre::clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rtre::camera.updatePerspective();
		my.draw();

		WGLWindow::pollEvents();
		window.swapBuffers();
		rtre::controller::control();
	}

	WGLWindow::terminate();
}
