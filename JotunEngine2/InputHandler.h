#pragma once
class InputHandler {
public:
	virtual void onKeyPress( int key ) = 0;
	virtual void onKeyRelease( int key ) = 0;
	virtual void onMouseMove( int deltaX, int deltaY ) = 0;
};

