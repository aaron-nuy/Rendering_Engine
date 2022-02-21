#pragma once
#include "glfw3.h"
#include <string>

struct WGLSize {
private:
	int m_height;
	int m_width;
public:

	WGLSize(int width, int height);

	int height();
	int width();
};
struct WGLCoords {
private:
	double m_x;
	double m_y;
public:

	WGLCoords(double x, double y);

	double x();
	double y();
};


class WGLWindow {

	GLFWwindow* m_Window;

public:

	WGLWindow(int width, int height, const std::string& title);
	WGLWindow(WGLSize size, const std::string& title);

	static int init();
	static void terminate();
	static void initHint(int hint, int value);
	static void pollEvents();

	void setTitle(const std::string& title);

	void makeContextCurrent();
	void detachContext();

	void swapInterval(int value);
	void swapBuffers();

	double getTime();
	
	int shouldClose();
	void setShouldClose(int value);
	
	int isKeyPressed(int key);
	int isKeyReleased(int key);

	GLFWkeyfun setKeyCallback(GLFWkeyfun callback_handler);

	int isMousePressed();
	int isMouseReleased();

	WGLSize getWindowSize();
	void setWindowSize(WGLSize size);
	void setWindowSize(int width,int height);

	WGLCoords getCursorPosition();
	void setCursorPosition(double x, double y);
	void setCursorPosition(WGLCoords coords);
	void setInputMode(int mode, int value);

	WGLCoords getWindowPosition();
	void setWindowPosition(WGLCoords coords);
	void setWindowPosition(int x,int y);
};