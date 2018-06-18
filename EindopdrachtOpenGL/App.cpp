#pragma once
#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include "Utility.h"
#include "KeyHandler.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

using namespace std;

//set parameter to true for a fullscreen application
static bool FULLSCREEN = false;
GLFWwindow* gWindow = nullptr;

//vertex buffer object, array object & shader
GLuint vbo, vao, ibo;
ShaderProgram shaderProgram;
Texture2D texture1, texture2;
const std::string texturePath1 = "airplane.png";
const std::string texturePath2 = "crate.jpg";


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
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);

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

void InitVertices() {
	//quad
	GLfloat vertices[] = {
		//position				texture coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,		// Top left
		0.5f,  0.5f, 0.0f,		1.0f, 1.0f,		// Top right
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		// Bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f		// Bottom left 
	};

	GLuint indices[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};

	//create memory in gpu
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//make current buffer object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	// Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Set up index buffer
	glGenBuffers(1, &ibo);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//load vertex & fragment shader
	shaderProgram.loadShaders("basic.vert", "basic.frag");

	//load textures
	texture1.loadTexture(texturePath1, true);
	texture2.loadTexture(texturePath2, true);

}

int main() {

	if (!initOpenGL()) {
		return -1;
	}

	InitVertices();

	//main loop
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);

		//polls for events like keypresses
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		texture1.bind(0);
		texture2.bind(1);
		shaderProgram.use();
		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler1"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler2"), 1);


		//bind vertex array object to draw
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//unbind array
		glBindVertexArray(0);
		glfwSwapBuffers(gWindow);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	glfwTerminate();
	return 0;
}