#include "stdafx.h"
#include "Transform.h"

Transform::Transform(void) : position( 0, 0, 0 ), rotation( 0, 0, 0 ), 
	forward( 0, 0, 1 ), up( 0, 1, 0 ), right( 1, 0, 0 )
{}

Transform::Transform( Vector3 pos, Vector3 rot ) : position( pos ), rotation( rot ), 
	forward( 0, 0, 1 ), up( 0, 1, 0 ), right( 1, 0, 0 )
{}

Transform::~Transform(void)
{}

void Transform::setPosition( Vector3 pos ) {
	position = pos;
}

void Transform::setPosition( float newX, float newY, float newZ ) {
	position.set( newX, newY, newZ );
}

void Transform::setRotation( Vector3 rot ) {
	rotation = rot;
	calculateAxis();
}

void Transform::calculateAxis() {
	//to be filled in later when I can 3d math
}

void Transform::moveBy( Vector3 dist ) {
	position += dist;
}

void Transform::rotate( Vector3 rot ) {
	rotation += rot;
}

Vector3 Transform::getPosition() {
	return position;
}

Vector3 Transform::getRotation() {
	return rotation;
}

Vector3 Transform::getForward() {
	return forward;
}

Vector3 Transform::getRight() {
	return right;
}

Vector3 Transform::getUp() {
	return up;
}