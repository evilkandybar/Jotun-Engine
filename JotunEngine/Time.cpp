#include "stdafx.h"
#include "Time.h"

double Time::time = 0;
double Time::deltaTime = 0;
double Time::framerate = 0;
double Time::lastClock = 0;

void Time::update() {
	double curClock = double(clock()) / double( CLOCKS_PER_SEC );
	deltaTime = curClock - lastClock;
	time += deltaTime;
	framerate = 1.0 / deltaTime;
	lastClock = curClock;
}