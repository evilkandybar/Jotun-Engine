#pragma once
#include "Input.h"
#include "Vector3.h"
#include "Time.h"
#include "Transform.h"
class Camera
{
public:
	Camera(void);
	Camera( float newFOV, int newScreenX, int newScreenY, Vector3 pos, Vector3 rot );
	~Camera(void);
	void setPosition( Vector3 pos );
	void setPosition( float newX, float newY, float newZ );
	void setRotation( Vector3 rot );
	void moveBy( Vector3 dist );
	void rotateBy( Vector3 rot );
	void lookAt( Vector3 pos );
	void setFarClip( float newFarClip );
	void setNearClip( float newNearClip );
	void setFOV( float newFOV );
	void setAspectRatio( float newAR );
	void setScreenX( int newScreenX );
	void setScreenY( int newScreenY );
	void genMatrixes();
	void setMatricies();
	void setActive();
	void update();
	void prepare();
	Transform& getTransform();
	aiMatrix4x4& getProjectionMatrix();
	aiMatrix4x4& getViewMatrix();
protected:
	enum MODE {
		PERSPECTIVE = 0,
		ORTHOGRAPHIC = 1,
	} projMode;
	float fov, nearClip, farClip, aspectRatio;
	int screenX, screenY;
	aiMatrix4x4 projectionMatrix, viewMatrix;
	Vector3 camTarget;
	Transform transform;
};

extern aiMatrix4x4 floatsToMatrix( float* data );
extern float* matrixToFloats( aiMatrix4x4 );