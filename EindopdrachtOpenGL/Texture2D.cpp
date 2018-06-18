#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture2D.h"

Texture2D::Texture2D()
{
	mTexture = 0;
}
Texture2D::~Texture2D() = default;

bool Texture2D::loadTexture(const string & filename, bool generateMipMaps)
{
	int width, height, components;

	// Load the image through stb library
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == nullptr) {
		cout << current_exception << "error loading texture" << filename << "'" << endl;
		return false;
	}

	//invert image so it won't be upside down
	int widthInBytes = width * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;
	for (int row = 0; row < halfHeight; row++)
	{
		top = imageData + row * widthInBytes;
		bottom = imageData + (height - row - 1) * widthInBytes;
		for (int col = 0; col < widthInBytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	// Generate & bind textures
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//If texture doesn't fit use linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	if (generateMipMaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//free up memory & unbind texture
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void Texture2D::bind(GLuint texUnit)
{
	assert(texUnit >= 0 && texUnit < 32);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
