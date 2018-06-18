#pragma once
#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include "Utility.h"
#include "KeyHandler.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "gtc/matrix_transform.hpp"

using namespace std;

//set parameter to true for a fullscreen application
static bool FULLSCREEN = false;
GLFWwindow* gWindow = nullptr;

//vertex buffer object, array object & shader
GLuint vbo, vao, ibo;
ShaderProgram shaderProgram;
Texture2D texture1, texture2;
const std::string texturePath = "wooden_crate.jpg";
//const std::string texturePath2 = "crate.jpg";

glm::vec3 cubPos;

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
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);
	return true;
	
}

void InitVertices() {
	//cube
	GLfloat vertices[] = {
		// front face
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		// left face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// right face
		1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		// top face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		// bottom face
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	};

	//create memory in gpu
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//make current buffer object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//position coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	// Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//load vertex & fragment shader
	shaderProgram.loadShaders("basic.vert", "basic.frag");

	//load textures
	texture1.loadTexture(texturePath, true);
	//texture2.loadTexture(texturePath2, true);

}

int main() {

	if (!initOpenGL()) {
		return -1;
	}

	InitVertices();

	//cube position, right infront, 5 away from camera
	cubPos = glm::vec3(0.0f, 0.0f, 5.0f);

	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();
	
	//main loop
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;


		//polls for events like keypresses
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind(0);
		//texture2.bind(1);

		cubeAngle += (float)(deltaTime*50.0f);
		if(cubeAngle >= 360)
			cubeAngle = 0.0f;

		// configure camera
		glm::mat4 model, view, projection;
		model = glm::translate(model, cubPos)* glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f); 
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(camPos, targetPos, up);
		projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		shaderProgram.use();
		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);


		//bind vertex array object to draw
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//unbind array
		glBindVertexArray(0);
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}