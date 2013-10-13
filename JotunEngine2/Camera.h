#pragma once
#include "stdafx.h"
#include "Time.h"
class Camera : public InputHandler, protected Transformable {
public:
	Camera();
	Camera( glm::vec3 initPos );
	~Camera();
	
	void		lookAt( glm::vec3 pos );
	void		update();

	void		setFOV( float newFOV );
	void		setAspectRatio( float newAspectRatio );
	void		rotate( float hRot, float vRot );

	virtual void updateTrans();
	virtual void translate( glm::vec3 dir );
	virtual glm::vec3 getPosition();
	virtual glm::vec3 getForward();
	virtual glm::vec3 getUp();
	virtual glm::vec3 getRight();

	virtual void onKeyPress( int key );
	virtual void onKeyRelease( int key );
	virtual void onMouseMove( int deltaX, int deltaY );

	float		getFOV();
	glm::mat4	getViewMatrix();
	glm::mat4	getProjMatrix();
private:
	void init();

	//all angles are in radians
	float fov, aspectRatio, verticalAngle, horizontalAngle;
	float mouseSpeed;
	glm::mat4 viewMatrix, projMatrix;
};

