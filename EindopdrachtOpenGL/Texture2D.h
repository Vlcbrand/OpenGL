#pragma once
#include <string>
#include "glew.h"
#include <iostream>

using namespace std;

class Texture2D
{
private:
	GLuint mTexture;

public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const string& filename, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);
};

