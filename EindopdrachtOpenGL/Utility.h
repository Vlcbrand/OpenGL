#pragma once
#include "glew.h"
#include "glfw3.h"
static char APPTITLE[] = "OpenGL eindopdracht Vincent Brands";

GLFWwindow* InitWindow(bool fullcreen);
void showFPS(GLFWwindow *window);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
