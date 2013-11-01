#pragma once
#include "Light.h"
class DirectionalLight :
	public Light {
public:
	DirectionalLight( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition  );
	~DirectionalLight();
	virtual void update();
	virtual glm::vec4 getPos();
};

