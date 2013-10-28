#pragma once
#include "stdafx.h"
#include "Time.h"
//********//********//********//********//********//********//********//********
/*!\brief Defines a camera for use in rendering.

The camera inherits from InputHandler so it can move in response to player input
and inherits from Transformable so it has a position, rotation, and the like*/
class Camera : public InputHandler, public Transformable {
public:
	/*!\brief Default contructor
	
	Values are initialized as follows:
	Field of view: 60
	Aspect ratio: 4:3
	Vertical angle: 0
	Horizontal angle: PI
	*/
	Camera();
	/*!brief Gives the camera an initial position, although the default alignment 
	is used.

	Values are initialized as follows:
	Field of view: 60
	Aspect ratio: 4:3
	Vertical angle: 0
	Horizontal angle: PI
	
	\param [in] initPos The desired initial position of the camera*/
	Camera( glm::vec3 initPos );
	/*!\brief Destructor*/
	~Camera();
	
	/*!\brief Computes the horozontal and vertical rotation of the camera so that
	it is looking in the specified direction
	
	\param [in] pos The position that the camera should look at*/
	void		lookAt( glm::vec3 pos );
	/*!\brief Checks for the camera's movement flags, moving the camera 
	accordingly*/
	void		update();

	/*!\brief Sets the camera's field of view to the specified value
	
	\param [in] newFOV The desired field of view of the camera*/
	void		setFOV( float newFOV );
	/*!\brief Sets the camera's aspect ratio to the specified value
	
	\param [in] newAspectRatio The desired aspect ratio for the camera*/
	void		setAspectRatio( float newAspectRatio );
	/*!\brief Rotates the camera by the specified vertical and horozontal 
	rotations
	
	\param [in] hRot The amount of horozontal rotation to add to the camera
	\param [in] vRot The amount of vertical rotation to add to the camera*/
	void		rotate( float hRot, float vRot );

	/*!\brief Sets the camera's rotation to the specified value
	
	\param [in] hRot The desired horozontal rotation of the camera
	\param [in] vRot the desired vertical rotation of the camera*/
	void		setRotation( float hRot, float vRot );

	/*!\brief Updates values inherited from Transformable, calculating the
	forward, up, and right direction*/
	virtual void updateTrans();

	/*!\brief handles a key press for a camera
	
	If the key is W, the camera begins moving forward
	If the key is A, the camera begins moving left
	If the key is S, the camera begins moving back
	If the key is D, the camera begins moving right
	If the key is R, the camera begins moving up
	If the key is F, the camera begins moving down
	
	All movements should be relative to the camera's alignment, but are actually
	relative to the world's alignment

	This function is called from the key callback specified in main.cpp
	
	\param [in] key The key which was pressed*/
	virtual void onKeyPress( int key );
	/*!\brief handles a key release for a camera

	If the key is W, the camera stops moving forward
	If the key is A, the camera stops moving left
	If the key is S, the camera stops moving back
	If the key is D, the camera stops moving right
	If the key is R, the camera stops moving up
	If the key is F, the camera stops moving down

	All movements should be relative to the camera's alignment, but are actually
	relative to the world's alignment

	This function is called from the key callback specified in main.cpp

	\param [in] key The key which was released*/
	virtual void onKeyRelease( int key );
	/*!\brief Handles a mouse movement for a camera
	
	The camera turns to follow the mouse as it moves around the screen
	
	\param [in] mouseX The current screen x of the mouse
	\param [in] mouseY The current screen y of the mouse*/
	virtual void onMouseMove( int mouseX, int mouseY );

	/*!\brief Returns the current field of view for the camera
	
	\return The camera's current vield of view*/
	float		getFOV();
	/*!\brief Returns the camera's view matrix, calculated from the camera's 
	position and forward direction
	
	\return The camera's view matrix*/
	glm::mat4	getViewMatrix();
	/*!\brief returns the camera's projection matrix, calculated from the field
	of view, aspect ratio, and default values for the near and far clipping 
	planes
	
	\return The camera's projection matrix*/
	glm::mat4	getProjMatrix();
private:
	void init();

	//all angles are in radians
	float fov, aspectRatio, verticalAngle, horizontalAngle;
	float mouseSpeed;
	glm::mat4 viewMatrix, projMatrix;
	bool moveUp, moveDown, moveRight, moveLeft, moveForward, moveBack;
};

