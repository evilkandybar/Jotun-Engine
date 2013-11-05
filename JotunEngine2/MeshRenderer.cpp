#include "MeshRenderer.h"

MeshRenderer::MeshRenderer( Mesh *newMesh, Shader *newShader ) {
	mesh = newMesh;
	shader = newShader;
	shouldChangeMesh = false;
	shouldChangeShader = false;
}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::render( glm::mat4 projectionMatrix, glm::mat4 viewMatrix ) {
	if( shouldChangeShader ) {
		shader->bind();
	}
	if( shouldChangeMesh ) {
		mesh->bind( shader->getAttribute( "vertexPosition_worldspace" ),
					shader->getAttribute( "vertexUV" ), 
					shader->getAttribute( "vertexNormal_worldspace" ),
					shader->getAttribute( "vertexTangent_worldspace" ) );
	}
	mesh->draw();
}