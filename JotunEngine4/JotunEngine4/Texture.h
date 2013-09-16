#include "stdafx.h"

#pragma once
class Texture
{
public:
	Texture(void);
	Texture( std::string );
	~Texture(void);
	void giveName( std::string newName );
	void giveID( GLuint newID );
	void giveData( GLfloat *newData );
	void setDimensions( GLuint newX, GLuint newY );
private:
	int x, y, glid, numChannels;
	std::string name;
	GLubyte *data;
};

