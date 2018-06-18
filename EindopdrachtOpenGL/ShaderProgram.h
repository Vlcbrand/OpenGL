#pragma once
#include <iostream>
#include <glew.h>
#include <glm.hpp>
#include <map>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& vec2);
	void setUniform(const GLchar* name, const glm::vec3& vec3);
	void setUniform(const GLchar* name, const glm::vec4& vec4);
	void setUniform(const GLchar* name, const glm::mat4& mat4);


	inline GLuint getProgram() const {
		 return mHandle;
	}

private:
	std::string fileToString(const std::string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);


	GLuint mHandle;
	std::map<std::string, GLint> mUniformLocations;
};

