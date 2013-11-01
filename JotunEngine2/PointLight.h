#pragma once
#include "Light.h"
class PointLight :
	public Light {
public:
	PointLight( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition );
	~PointLight();
};

