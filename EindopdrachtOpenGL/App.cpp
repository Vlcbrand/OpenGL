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

//vertex buffer object, array object
GLuint vbo, vao, shaderProgram;

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"out vec3 vert_color;"
"void main()"
"{"
"	vert_color = color;"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);"
"}";

const GLchar* fragmentSharderSrc =
"#version 330 core\n"
"in vec3 vert_color;"
"out vec4 frag_color;"
"void main()"
"{"
"    frag_color = vec4(vert_color, 1.0f);"
"}";


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
	GLfloat vertices[] =
		// position			// color
	{   0.0f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// top
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	// right	
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// left
	};

	//create memory in gpu
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//make current 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position										24 bytes so *6
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, NULL);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid * )(sizeof(GLfloat)*3));
	glEnableVertexAttribArray(1);

	//vertexshader info
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	//fragmentshader info
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentSharderSrc, NULL);
	glCompileShader(fs);

	//link shaders to program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	//shaders already linked
	glDeleteShader(vs);
	glDeleteShader(fs);
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

		//use shader program
		glUseProgram(shaderProgram);
		//draw
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(gWindow);
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);


	glfwTerminate();
	return 0;
}