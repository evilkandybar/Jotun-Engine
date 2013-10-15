#include "Input.h"

bool Input::keys[349];
double Input::mouseX = 0;
double Input::mouseY = 0;
double Input::mouseDeltaX = 0;
double Input::mouseDeltaY = 0;

void Input::clear() {
	for( int i = 0; i < 349; i++ ) {
		keys[i] = false;
	}
}

void Input::updateMouse( double newMouseX, double newMouseY ) {
	mouseDeltaX = newMouseX - mouseX;
	mouseDeltaY = newMouseY - mouseY;
	mouseX = newMouseX;
	mouseY = newMouseY;
}