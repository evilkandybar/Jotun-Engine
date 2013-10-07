#line 1 "d:\\my documents\\programs\\jotunengine2\\inputhandler.cpp"
#line 1 "d:\\my documents\\programs\\jotunengine2\\inputhandler.h"
#pragma once
class InputHandler {
public:
	virtual void onKeyPress( int key ) = 0;
	virtual void onKeyRelease( int key ) = 0;
	virtual void onMouseMove( int deltaX, int deltaY ) = 0;
};

#line 2 "d:\\my documents\\programs\\jotunengine2\\inputhandler.cpp"
