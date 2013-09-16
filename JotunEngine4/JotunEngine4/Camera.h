#include "stdafx.h"

#pragma once
class Camera
{
public:
	Camera(void);
	~Camera(void);
	glm::mat4 transformModel( glm::mat4 &model );
private:
	glm::mat4 view, projection;
};

