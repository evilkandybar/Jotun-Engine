#include "Light.h"

Light::Light() {
}

Light::Light( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition ) {
	this->position = newPosition;
	this->color = glm::vec4( newColor, intensity );
}

Light::~Light() {}

void Light::setColor( glm::vec3 &newColor ) {
	color = glm::vec4( newColor, color.a );
}

void Light::setIntensity( GLfloat intensity ) {
	color.a = intensity;
}

void Light::setPosition( glm::vec3 &newPos ) {
	position = newPos;
}

glm::mat4 &Light::getMatrix( glm::vec3 &screenCenter ) {
	return glm::lookAt( position, screenCenter, glm::vec3( 0, 1, 0 ) );
}

glm::vec3 &Light::getPos() {
	return position;
}

glm::vec4 &Light::getColor() {
	return color;
}