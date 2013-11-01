#include "DirectionalLight.h"


DirectionalLight::DirectionalLight( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition ) :
	Light( newColor, intensity, newPosition ) 
{
	shadowCam->setFOV( 0 );
}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::update() {
	shadowCam->update();
}

glm::vec4 DirectionalLight::getPos() {
	return glm::vec4( position, 0 );
}