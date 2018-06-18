#pragma once
#include "Utility.h"
#include <sstream>

GLFWwindow* InitWindow(bool fullscreen) {

	// handle versions for opengl (min version 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (fullscreen) {
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVidMode = glfwGetVideoMode(pMonitor);

		return glfwCreateWindow(pVidMode->width, pVidMode->height, APPTITLE, pMonitor, NULL);
	}
	else
		return glfwCreateWindow(800, 600, APPTITLE, NULL, NULL);
}

// shows fps in non windowed mode
void showFPS(GLFWwindow * window){
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elasped;
	double current = glfwGetTime(); // Returns numers of seconds since GLFW started

	elasped = current - previousSeconds;

	// Limit update to 4 times per second
	if (elasped > 0.25) {
		previousSeconds = current;
		double fps = (double)frameCount / elasped;
		double msPerFrame = 1000.0 / fps;

		 //show fps and framecount on the title of the app
		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed << APPTITLE << "       FPS : " << fps << "       Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}
	frameCount++;
}
