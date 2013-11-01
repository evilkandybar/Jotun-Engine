#include "Light.h"

Light::Light() {
}

Light::Light( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition ) {
	position = newPosition;
	color = glm::vec4( newColor, intensity );
	shadowCam = new Camera( position );
	shadowCam->setAspectRatio( 1 );

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers( 1, &framebufferName );
	glBindFramebuffer( GL_FRAMEBUFFER, framebufferName );

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures( 1, &depthTexture );
	glBindTexture( GL_TEXTURE_2D, depthTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTexture, 0 );

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

Light::~Light() {
	glDeleteFramebuffers( 1, &framebufferName );
	glDeleteTextures( 1, &depthTexture );
	delete shadowCam;
}

void Light::update() {
	shadowCam->update();
}

void Light::setColor( glm::vec3 &newColor ) {
	color = glm::vec4( newColor, color.a );
}

void Light::setIntensity( GLfloat intensity ) {
	color.a = intensity;
}

void Light::setPosition( glm::vec3 &newPos ) {
	position = newPos;
}

GLuint Light::getDepthTexture() {
	return depthTexture;
}

glm::mat4 &Light::getViewMatrix( glm::vec3 &screenCenter ) {
	shadowCam->lookAt( screenCenter );
	return shadowCam->getViewMatrix();
}

glm::mat4 &Light::getProjMatrix() {
	return shadowCam->getProjMatrix();
}

glm::vec4 &Light::getColor() {
	return color;
}