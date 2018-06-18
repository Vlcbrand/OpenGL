#pragma once
#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include "Utility.h"
#include "KeyHandler.h"
#include "ShaderProgram.h"

using namespace std;

//set parameter to true for a fullscreen application
static bool FULLSCREEN = false;
GLFWwindow* gWindow = nullptr;

//vertex buffer object, array object & shader
GLuint vbo, vao, ibo;
ShaderProgram shaderProgram;


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

void InitTriangle() {
	//Triangle
	GLfloat vertices[] = {
		-0.5f,  0.5f, 0.0f,		// Top left
		0.5f,  0.5f, 0.0f,		// Top right
		0.5f, -0.5f, 0.0f,		// Bottom right
		-0.5f, -0.5f, 0.0f		// Bottom left 
	};

	GLuint indices[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};

	//create memory in gpu
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//make current 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);	
	glEnableVertexAttribArray(0);

	// Set up index buffer
	glGenBuffers(1, &ibo);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shaderProgram.loadShaders("basic.vert", "basic.frag");
}

int main() {

	if (!initOpenGL()) {
		return -1;
	}

	InitTriangle();

	//main loop
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);

		//polls for events like keypresses
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		shaderProgram.use();

		//change uniform variables in the shaders. (1 for offset, 1 for color)
		GLfloat time = (GLfloat)glfwGetTime();
		GLfloat blueColor = (sin(time) / 2) + 0.5f;
		glm::vec2 pos;
		pos.x = sin(time) / 2;
		pos.y = cos(time) / 2;
		shaderProgram.setUniform("vertColor", glm::vec4(0.0f, 0.0f, blueColor, 1.0f));
		shaderProgram.setUniform("posOffset", pos);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(gWindow);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	glfwTerminate();
	return 0;
}