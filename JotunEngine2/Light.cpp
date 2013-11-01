#include "Light.h"

Light::Light() {
}

Light::Light( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition ) {
	position = newPosition;
	color = glm::vec4( newColor, intensity );
	shadowCam = new Camera( position );
	shadowCam->setAspectRatio( 1 );
}

Light::~Light() {
	delete shadowCam;
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