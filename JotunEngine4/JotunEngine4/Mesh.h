#pragma once
#include "stdafx.h"
class Mesh
{
public:
	Mesh(std::string mName);
	~Mesh();
	void draw();
private:
	void loadOBJ(std::string fileToLoad);
	void makeGLMesh();
	void bind();

	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint numVerts;
	GLfloat *vertexes, *texCoords, *normals;
	std::string name;
};

