#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transformable.h"
class MeshRenderer :
	public Transformable
{
public:
	MeshRenderer( Mesh *newMesh, Shader *newShader );
	~MeshRenderer();
	void render( glm::mat4 projectionMatrix, glm::mat4 viewMatrix );
private:
	bool shouldChangeMesh;
	bool shouldChangeShader;
	Mesh *mesh;
	Shader *shader;
};

/*
The MeshRenderer knows its shader. We can grab it and compare it with the previous
shader. If it's different, do the bind thing. Set the uniforms. Each model will
have a seperate transform, so we need to give the MeshRenderer 
*/