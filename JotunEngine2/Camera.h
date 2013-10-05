#pragma once
#include "stdafx.h"
class Camera {
public:
	Camera();
	Camera( glm::vec3 initPos );
	~Camera();
	
	void		lookAt( glm::vec3 pos );
	void		update();

	void		setFOV( float newFOV );
	void		setAspectRatio( float newAspectRatio );

	float		getFOV();
	glm::vec3	getPosition();
	glm::vec3	getForward();
	glm::vec3   getUp();
	glm::vec3	getRight();
	glm::mat4	getViewMatrix();
	glm::mat4	getProjMatrix();
private:
	void init();

	//all angles are in radians
	float fov, aspectRatio, verticalAngle, horozontalAngle;
	glm::vec3 position, up, right, forward;
	glm::mat4 viewMatrix, projMatrix;
};

