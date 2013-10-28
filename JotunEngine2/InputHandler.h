#pragma once
/*!\brief Defines an interface which can be used by any object to capture and
handle input*/
class InputHandler {
public:
	/*!\brief Allows an object to handle a key press
	
	\param [in] key The key which was pressed*/
	virtual void onKeyPress( int key ) = 0;
	/*!\brief Allows an object to handle a key release
	
	\param [in] key The key which was released*/
	virtual void onKeyRelease( int key ) = 0;
	/*!\brief Alows an object to handle mouse movement
	
	\param [in] deltaX The amount that the mouse moved by in the x-direction
	\param [in] deltaY The amount that the mouse moved by in the y-direction*/
	virtual void onMouseMove( int deltaX, int deltaY ) = 0;
};

