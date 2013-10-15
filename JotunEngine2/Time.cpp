#include "Time.h"

float Time::lastTime = 0;
float Time::curTime = 0;
float Time::deltaTime = 0;

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