#pragma once
#include "stdafx.h"
class Texture {
public:
	Texture( std::string name );
	~Texture();
	void		bind( int unit );
	GLuint		getGLName() { return glName; };
private:
	void		loadTexture();
	void		makeGLTex();
	int			width, height, comp;
	std::string sName;
	GLubyte		*data;
	GLuint		glName;
};

