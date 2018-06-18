#pragma once
#include <iostream>

#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"

#include "Utility.h"
#include "KeyHandler.h"

using namespace std;

int main() {

	if (!glfwInit()) {
		cerr << "GLFW cant init" << endl;
		return -1;
	}
	
	// SET PARAMETER TO TRUE FOR FULL SCREEN
	GLFWwindow *pWindow = InitWindow(false);
	if (pWindow == nullptr) {
		cerr << "Window cant init" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	glfwSetKeyCallback(pWindow, OnKey);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "GLEW cant init" << endl;
		glfwTerminate();
		return -1;
	}

	//main loop
	while (!glfwWindowShouldClose(pWindow)) {
		showFPS(pWindow);

		//polls for events like keypresses
		glfwPollEvents();

		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}