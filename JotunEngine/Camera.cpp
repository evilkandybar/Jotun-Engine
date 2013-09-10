#include "stdafx.h"
#include "Camera.h"

Camera::Camera(void) {
	fov = 60;
	nearClip = 0.01f;
	farClip = 1000;
}

Camera::~Camera(void) {}

Camera::Camera( float newFOV, int newScreenX, int newScreenY, Vector3 pos, Vector3 rot ) : transform( pos, rot ) {
	fov = newFOV;
	screenX = newScreenX;
	screenY = newScreenY;
	aspectRatio = float(screenX)/float(screenY);
	nearClip = 0.01f;
	farClip = 1000;
}

void Camera::setPosition( Vector3 pos ) {
	transform.setPosition( pos );
	genMatrixes();
}

void Camera::setPosition( float newX, float newY, float newZ ) {
	transform.setPosition( newX, newY, newZ );
}

void Camera::moveBy( Vector3 dist ) {
	transform.moveBy( dist );
	genMatrixes();
}

void Camera::setRotation( Vector3 rot ) {
	transform.setRotation( rot );
	genMatrixes();
}

void Camera::rotateBy( Vector3 rot ) {
	transform.rotate( rot );
	genMatrixes();
}

void Camera::lookAt( Vector3 pos ) {
	camTarget = pos;
	gluLookAt( transform.getPosition().getX(), transform.getPosition().getY(), transform.getPosition().getZ(),
		camTarget.getX(), camTarget.getY(), camTarget.getZ(),
		0, 1, 0 );
	GLfloat *matrixPointer = new GLfloat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, matrixPointer );
	projectionMatrix = floatsToMatrix( matrixPointer );
	glGetFloatv( GL_PROJECTION_MATRIX, matrixPointer );
	viewMatrix = floatsToMatrix( matrixPointer );
}

void Camera::setFarClip( float newFarClip ) {
	farClip = newFarClip;
}

void Camera::setNearClip( float newNearClip ) {
	nearClip = newNearClip;
}

void Camera::setFOV( float newFOV ) {
	fov = newFOV;
}

void Camera::setAspectRatio( float newAR ) {
	aspectRatio = newAR;
}

void Camera::setScreenX( int newScreenX ){
	screenX = newScreenX;
	aspectRatio = float(screenX)/float(screenY);
	
}

void Camera::setScreenY( int newScreenY ) {
	screenY = newScreenY;
	aspectRatio = float(screenX)/float(screenY);
}

void Camera::genMatrixes() {
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	gluPerspective( fov, aspectRatio, 0.01f, 100 );
	GLfloat *matrixPointer = new GLfloat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, matrixPointer );
	projectionMatrix = floatsToMatrix( matrixPointer );

	glLoadIdentity();
	glTranslatef( transform.getPosition().getX(), transform.getPosition().getY(), transform.getPosition().getZ() );
	glGetFloatv( GL_MODELVIEW_MATRIX, matrixPointer );
	viewMatrix = floatsToMatrix( matrixPointer );
	glPopMatrix();
}

void Camera::setMatricies() {
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( matrixToFloats( viewMatrix ) );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( matrixToFloats( projectionMatrix ) );
}

void Camera::setActive() {
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( fov, float(screenX)/float(screenY), 0.01, 1000 );
	glViewport( 0, 0, screenX, screenY );
}

void Camera::prepare() {
	Vector3 position = transform.getPosition();
	Vector3 rotation = transform.getRotation();
	glTranslatef( position.getX(), position.getY(), position.getZ() );
	glRotatef( rotation.getX(), 1, 0, 0 );
	glRotatef( rotation.getY(), 0, 1, 0 );
	glRotatef( rotation.getZ(), 0, 0, 1 );
}

void Camera::update() {
	if( Input::w ) {
		transform.moveBy( transform.getUp() * -Time::deltaTime );
	} else if( Input::s ) {
		transform.moveBy( transform.getUp() * Time::deltaTime );
	}
	if( Input::a ) {
		transform.moveBy( transform.getRight() * -Time::deltaTime );
	} else if( Input::d ) {
		transform.moveBy( transform.getRight() * Time::deltaTime );
	}
	if( Input::r ) {
		transform.moveBy( transform.getForward() * Time::deltaTime );
	} else if( Input::f ) {
		transform.moveBy( transform.getForward() * -Time::deltaTime );
	}
}

Transform& Camera::getTransform() {
	return transform;
}

aiMatrix4x4& Camera::getProjectionMatrix() {
	return projectionMatrix;
}

aiMatrix4x4& Camera::getViewMatrix() {
	return viewMatrix;
}