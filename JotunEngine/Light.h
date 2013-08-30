#pragma once

#include "Camera.h"
#include "Color.h"
#include "Shaders.h"
#include "Vector3.h"

//Currently simulates a point light. Will eventually simulate an arbitrary mesh light
//Not sure how to make a flat out directional light
class Light
{
public:
	Light(void);
	Light( float newX, float newY, float newZ, 
		float newR, float newG, float newB, float newA );
	~Light(void);

	void enable();
	void disable();
	void update();
	void drawGizmo();

	void setPosition( Vector3 );
	void setPosition( float x, float y, float z );
	void setColor( Color );
	void setColor( float r, float g, float b, float a );
	void setColor( float *data );
	void prepareShadowCam();
	Camera& getCamera();
protected:
	static int glNameCounter;
	unsigned int glName;
	Color color;	//color.getA() is intensity
	Vector3 position;
	Camera shadowCam;
};

