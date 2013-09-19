#include "stdafx.h"

#pragma once
class Texture
{
public:
	Texture(void);
	Texture( std::string );
	~Texture(void);
	void makeGLTexture();
	void bind();
	void giveName( std::string newName );
	void giveID( GLuint newID );
	void giveData( GLubyte *newData );
	void setDimensions( GLuint newX, GLuint newY );
	GLuint getX();
	GLuint getY();
	std::string getName();
	GLubyte* getData();
private:
	int x, y, numChannels;
	GLuint glid;
	std::string name;
	GLubyte *data;
};

