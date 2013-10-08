#pragma once
#include "stdafx.h"
#include "Time.h"
class Camera : public InputHandler {
public:
	Camera();
	Camera( glm::vec3 initPos );
	~Camera();
	
	void		lookAt( glm::vec3 pos );
	void		update();

	void		setFOV( float newFOV );
	void		setAspectRatio( float newAspectRatio );
	void		translate( glm::vec3 dir );
	void		rotate( float hRot, float vRot );

	virtual void onKeyPress( int key );
	virtual void onKeyRelease( int key );
	virtual void onMouseMove( int deltaX, int deltaY );

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
	float fov, aspectRatio, verticalAngle, horizontalAngle;
	float mouseSpeed;
	glm::vec3 position, up, right, forward;
	glm::mat4 viewMatrix, projMatrix;
};

