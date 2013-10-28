#include "Camera.h"

Camera::Camera() {
	Transformable::position = glm::vec3( 0, 0, 5 );
	init();
}

Camera::Camera( glm::vec3 initPos ) {
	Transformable::position = initPos;
	init();
}

Camera::~Camera() {}

void Camera::init() {
	fov = 60;
	aspectRatio = .75f;
	verticalAngle = 0;
	horizontalAngle = 3.14159;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
	mouseSpeed = 0.005f;
	moveUp = moveDown = moveRight = moveLeft = moveForward = moveBack = false;
}

void Camera::setFOV( float newFOV ) {
	fov = newFOV;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::setAspectRatio( float newAspectRatio ) {
	aspectRatio = newAspectRatio;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::rotate( float hRot, float vRot ) {
	horizontalAngle += hRot;
	verticalAngle += vRot;
}

void Camera::setRotation( float hRot, float vRot ) {
	horizontalAngle = hRot;
	verticalAngle = vRot;
}

void Camera::lookAt( glm::vec3 pos ) {
	viewMatrix = glm::lookAt( position, pos, up );
}

void Camera::update() {
	updateTrans();
	if( moveForward ) {
		translate( glm::vec3( 0, 0, -1 ) * Time::getDeltaTime() );
	} else if( moveBack ) {
		translate( glm::vec3( 0, 0, 1 ) * Time::getDeltaTime() );
	}
	if( moveUp ) {
		translate( glm::vec3( 0, 1, 0 ) * Time::getDeltaTime() );
	} else if( moveDown ) {
		translate( glm::vec3( 0, -1, 0 ) * Time::getDeltaTime() );
	}
	if( moveRight ) {
		translate( glm::vec3( -1, 0, 0 ) * Time::getDeltaTime() );
	} else if( moveLeft ) {
		translate( glm::vec3( 1, 0, 0 ) * Time::getDeltaTime() );
	}
	viewMatrix = glm::lookAt( position, position + forward, up );
}

void Camera::updateTrans() {
	//currently just calculates the forward, up, and right vectors and the relevant matricies
	forward = glm::vec3( cos( verticalAngle ) * sin( horizontalAngle ),
		sin( verticalAngle ), cos( verticalAngle ) * cos( horizontalAngle ) );

	right = glm::vec3( sin( horizontalAngle - 3.14f / 2.0f ),
		0, cos( horizontalAngle - 3.14f / 2.0f ) );

	up = glm::cross( right, forward );
}

void Camera::onKeyPress( int key ) {
	switch( key ) {
	case GLFW_KEY_A:
		moveRight = true;
		break;
	case GLFW_KEY_D:
		moveLeft = true;
		break; 
	case GLFW_KEY_W:
		moveForward = true;
		break;
	case GLFW_KEY_S:
		moveBack = true;
		break;
	case GLFW_KEY_R:
		moveUp = true;
		break;
	case GLFW_KEY_F:
		moveDown = true;
		break;
	}
}

void Camera::onKeyRelease( int key ) {
	switch( key ) {
	case GLFW_KEY_A:
		moveRight = false;
		break;
	case GLFW_KEY_D:
		moveLeft = false;
		break;
	case GLFW_KEY_W:
		moveForward = false;
		break;
	case GLFW_KEY_S:
		moveBack = false;
		break;
	case GLFW_KEY_R:
		moveUp = false;
		break;
	case GLFW_KEY_F:
		moveDown = false;
		break;
	}
}

void Camera::onMouseMove( int mouseX, int mouseY ) {
	setRotation( 3.14f + mouseSpeed * float(1024/2 - mouseX), mouseSpeed * float(768/2 - mouseY) );
}

float Camera::getFOV() {
	return fov;
}

glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() {
	return projMatrix;
}