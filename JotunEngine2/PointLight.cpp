#include "PointLight.h"

PointLight::PointLight( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition ) 
	: Light( newColor, intensity, newPosition ) 
{}

PointLight::~PointLight() {}
