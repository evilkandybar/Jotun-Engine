#include "stdafx.h"
#include "RenderMesh.h"

RenderMesh::RenderMesh(void)
{}

RenderMesh::RenderMesh( std::string sMeshName, std::string sShaderName ) {
	mesh = Meshes::loadMesh( sMeshName );
	shaderName = sShaderName;
	glShader = Shaders::loadShader( sShaderName );
}

RenderMesh::~RenderMesh(void) {
}

void RenderMesh::setShader() {
	glUseProgram( glShader );
}

void RenderMesh::setShader( GLuint s ) {
	glUseProgram( s );
}

void RenderMesh::draw() {
	mesh->render();
}