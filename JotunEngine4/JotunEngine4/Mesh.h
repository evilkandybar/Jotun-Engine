#include "stdafx.h"

#pragma once
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);
	void draw();
private:
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	static const GLfloat vertexArray[];
};

