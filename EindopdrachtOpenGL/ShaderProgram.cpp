#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() 
{
	mHandle = 0;
}


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(mHandle);
}


bool ShaderProgram::loadShaders(const char * vsFilename, const char * fsFilename)
{
	std::string vsString = fileToString(vsFilename);
	std::string fsString = fileToString(fsFilename);
	const GLchar* pvsSource = vsString.c_str();
	const GLchar* pfsSource = fsString.c_str();

	//vertexshader info
	GLint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &pvsSource, NULL);
	glCompileShader(vs);
	checkCompileErrors(vs);

	//fragmentshader info
	GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &pfsSource, NULL);
	glCompileShader(fs);
	checkCompileErrors(fs);

	//link shaders to program
	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	//shaders already linked
	glDeleteShader(vs);
	glDeleteShader(fs);
	
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

void ShaderProgram::checkCompileErrors(GLuint shader)
{
	int status = 0;
	GLint length = 0;

	glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

		std::string errorLog(length, ' ');
		glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
		std::cerr << "Error! Shader (program) failed to link. " << errorLog << std::endl;
	}

}
	