#include "Time.h"

void Time::init() {
	lastTime = glfwGetTime();
}

void Time::update() {
	curTime = glfwGetTime();
	deltaTime = curTime - lastTime;
	lastTime = curTime;
}

float Time::getTime() {
	return curTime;
}

float Time::getDeltaTime() {
	return deltaTime;
}