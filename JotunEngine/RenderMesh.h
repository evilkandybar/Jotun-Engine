#pragma once

#include "renderobject.h"
#include "Mesh.h"
#include "Meshes.h"
#include "Shaders.h"

class RenderMesh :
	protected RenderObject
{
public:
	RenderMesh(void);
	RenderMesh( std::string sMeshName, std::string sShaderName );	//tell the constructor which mesh and shader to use
	~RenderMesh(void);
	void draw();
	void setShader();
	void setShader( GLuint s );
protected:
	std::string shaderName;
	GLuint glShader;
	Mesh *mesh;
};

