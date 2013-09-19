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
	void bind();
	void uploadMatrix( glm::mat4 mvp );
	GLuint getID();
	std::string getName();
private:
	GLuint glid;
	GLuint mvpPos;
	std::string name;
};

