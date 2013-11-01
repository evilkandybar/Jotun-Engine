#include "DirectionalLight.h"


DirectionalLight::DirectionalLight( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition ) :
	Light( newColor, intensity, newPosition ) {
	shadowCam->setFOV( 0 );
}

DirectionalLight::~DirectionalLight() {}

glm::mat4 DirectionalLight::renderShadow( Mesh *meshes, int numMeshes, Shader *shader ) {
	glBindFramebuffer( GL_FRAMEBUFFER, framebufferName );
	glViewport( 0, 0, 1024, 1024 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glm::mat4 depthProjectionMatrix = glm::ortho<float>( -10, 10, -10, 10, -10, 20 );
	glm::mat4 depthViewMatrix = glm::lookAt( glm::vec3( position.x, position.y, position.z ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 ) );
	glm::mat4 depthModelMatrix = glm::mat4( 1.0 );
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix *
		depthModelMatrix;

	shader->bind();
	shader->setUniformMat4x4( "depthMVP", &depthMVP[0][0] );
	for( int i = 0; i < numMeshes; i++ ) {
		meshes[i].drawShadowPass( shader->getAttribute( "vertexPosition_modelspace" ) );
	}
	return depthMVP;
}

glm::vec4 DirectionalLight::getPos() {
	return glm::vec4( position, 0 );
}