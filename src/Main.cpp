#include <vector>
#include "rtre.h"
#include "GLFW/ar_WGL.h"
#include "engine_movement/controller.h"

int main()
{


	WGLWindow::init();
	WGLWindow::initHint(GLFW_CONTEXT_VERSION_MAJOR,4 );
	WGLWindow::initHint(GLFW_CONTEXT_VERSION_MINOR,3 );
	WGLWindow::initHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	WGLWindow window(500, 500,"Hey you");
	window.makeContextCurrent();



	rtre::init(500,500,&window);
	

	while (!window.shouldClose()) {
		rtre::setBackgroundColor(0.5, 0.1, 0.1,1.0);
		rtre::clearBuffers(GL_COLOR_BUFFER_BIT);


		rtre::controller::control();
		window.swapBuffers();
		WGLWindow::pollEvents();
	}

	WGLWindow::terminate();
}
