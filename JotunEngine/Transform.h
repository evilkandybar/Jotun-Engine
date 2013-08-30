#pragma once
#include "Vector3.h"
class Transform
{
public:
	Transform(void);
	Transform( Vector3 pos, Vector3 rot );
	~Transform(void);
	void setPosition( Vector3 pos );
	void setPosition( float newX, float newY, float newZ );
	void setRotation( Vector3 rot );
	void calculateAxis();
	void moveBy( Vector3 dist );
	void rotate( Vector3 rot );
	Vector3 getPosition();
	Vector3 getRotation();
	Vector3 getForward();
	Vector3 getRight();
	Vector3 getUp();
protected:
	Vector3 position, rotation, forward, right, up;
};

