#pragma once
#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include "Utility.h"
#include "KeyHandler.h"

using namespace std;

//set parameter to true for a fullscreen application
static bool FULLSCREEN = false;
GLFWwindow* gWindow = nullptr;

bool initOpenGL() {
	if (!glfwInit()) {
		cerr << "GLFW cant init" << endl;
		return false;
	}

	gWindow = InitWindow(FULLSCREEN);

	if (gWindow == nullptr) {
		cerr << "Window cant init" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gWindow);

	glfwSetKeyCallback(gWindow, OnKey);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "GLEW cant init" << endl;
		glfwTerminate();
		return false;
	}

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	return true;
}

int main() {

	if (!initOpenGL()) {
		return -1;
	}

	//main loop
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);

		//polls for events like keypresses
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(gWindow);
	}

	glfwTerminate();
	return 0;
}