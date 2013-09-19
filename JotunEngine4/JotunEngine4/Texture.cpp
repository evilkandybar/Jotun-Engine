#include "Texture.h"

Texture::Texture(void) {}

Texture::Texture( std::string newName ) {
	data = stbi_load( newName.c_str(), &x, &y, &numChannels, 0 );
	makeGLTexture();
}

Texture::~Texture(void) {}

void Texture::makeGLTexture() {
	glGenTextures(1, &glid);
	glBindTexture(GL_TEXTURE_2D, glid);
	
	if (numChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (numChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void bind() {
}

void Texture::giveName(std::string newName) {
	name = newName;
}

void Texture::giveData(GLubyte *newData) {
	data = newData;
	makeGLTexture();
}

void Texture::setDimensions(GLuint newX, GLuint newY) {
	x = newX;
	y = newY;
}

GLuint Texture::getX() {
	return x;
}

GLuint Texture::getY() {
	return y;
}

std::string Texture::getName() {
	return name;
}

GLubyte* Texture::getData() {
	return data;
}