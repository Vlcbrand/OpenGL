#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
ShaderProgram::ShaderProgram() 
{
	mHandle = 0;
}


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(mHandle);
}


bool ShaderProgram::loadShaders(const char * vsFilename, const char * fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);

	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	mHandle = glCreateProgram();
	if (mHandle == 0)
	{
		std::cerr << "Unable to create shader program!" << std::endl;
		return false;
	}

	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);

	glLinkProgram(mHandle);
	checkCompileErrors(mHandle, PROGRAM);


	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();

	return true;
}

void ShaderProgram::use()
{
	if (mHandle > 0) {
		glUseProgram(mHandle);
	}
}

std::string ShaderProgram::fileToString(const std::string & filename)
{
	std::stringstream ss;
	std::ifstream file;

	try {
		file.open(filename);
		if (!file.fail()) {
			// get content of file into stringstream
			ss << file.rdbuf();
		}
		file.close();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader program failed to link. " << errorLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string errorLog(length, ' '); 
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
		}
	}

}

GLint ShaderProgram::getUniformLocation(const GLchar * name)
{
	std::map<std::string, GLint>::iterator it = mUniformLocations.find(name);
	
	//check if it exists, if not get it and set it
	if (it == mUniformLocations.end())
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);

	return mUniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& vec2) {
	GLint location = getUniformLocation(name);
	glUniform2f(location, vec2.x, vec2.y);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& vec3) {
	GLint location = getUniformLocation(name);
	glUniform3f(location, vec3.x, vec3.y, vec3.z);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& vec4) {
	GLint location = getUniformLocation(name);
	glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
}
	