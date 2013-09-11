#include "stdafx.h"

#pragma once
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);
	void draw();
private:
	int vertexCount;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	static const GLfloat vertexArray[];
};

