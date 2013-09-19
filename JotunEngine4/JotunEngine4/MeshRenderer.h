#include "stdafx.h"
#include "MeshList.h"
#include "Shader.h"
#include "ShaderList.h"
#pragma once
class MeshRenderer
{
public:
	MeshRenderer( std::string mName, std::string sName );
	~MeshRenderer(void);
	void genBuffers();
	void draw();
private:
	int vertexCount;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	aiMesh *data;
	Shader *shader;
};

