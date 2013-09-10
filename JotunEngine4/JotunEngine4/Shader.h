#include "stdafx.h"

#pragma once
class Shader
{
public:
	Shader(void);
	Shader( GLuint newID );
	Shader( std::string newName );
	~Shader(void);
	void setID( GLuint newID );
	void setName( std::string newName );
	void setActive();
	GLuint getID();
	std::string getName();
private:
	GLuint glid;
	std::string name;
};

