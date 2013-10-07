#include "Camera.h"

Camera::Camera() : position( 0, 0, 5 ) {
	init();
}

Camera::Camera( glm::vec3 initPos ) : position( initPos ) {
	init();
}

void Camera::init() {
	fov = 60;
	aspectRatio = .75f;
	verticalAngle = 0;
	horizontalAngle = 3.14159;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::setFOV( float newFOV ) {
	fov = newFOV;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::setAspectRatio( float newAspectRatio ) {
	aspectRatio = newAspectRatio;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::translate( glm::vec3 dir ) {
	position += dir;
}

void Camera::rotate( float hRot, float vRot ) {
	horizontalAngle += hRot;
	verticalAngle += vRot;
}

void Camera::lookAt( glm::vec3 pos ) {
	viewMatrix = glm::lookAt( position, pos, up );
}

void Camera::update() {
	//currently just calculates the forward, up, and right vectors and the relevant matricies
	forward = glm::vec3( cos( verticalAngle ) * sin( horizontalAngle ),
		sin( verticalAngle ), cos( verticalAngle ) * cos( horizontalAngle ) );

	right = glm::vec3( sin( horizontalAngle - 3.14f / 2.0f ),
		0, cos( horizontalAngle - 3.14f / 2.0f ) );
	
	up = glm::cross( right, forward );

	viewMatrix = glm::lookAt( position, position + forward, up );
}

void Camera::onKeyPress( int key ) {
	switch( key ) {
	case GLFW_KEY_A:
		translate( glm::vec3( 1, 0, 0 ) * Time::getDeltaTime() );
		break;
	case GLFW_KEY_D:
		translate( glm::vec3( -1, 0, 0 ) * Time::getDeltaTime() );
		break; 
	case GLFW_KEY_W:
		translate( glm::vec3( 0, 1, 0 ) * Time::getDeltaTime() );
		break;
	case GLFW_KEY_S:
		translate( glm::vec3( 0, -1, 0 ) * Time::getDeltaTime() );
		break;
	}
}

void Camera::onKeyRelease( int key ) {}

void Camera::onMouseMove( int deltaX, int deltaY ) {
	rotate( 3.14f + mouseSpeed * float(deltaX), mouseSpeed * float(deltaY) );
}

float Camera::getFOV() {
	return fov;
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getForward() {
	return forward;
}

glm::vec3 Camera::getUp() {
	return up;
}

glm::vec3 Camera::getRight() {
	return right;
}

glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() {
	return projMatrix;
}