#pragma once
#include "stdafx.h"
class Time {
public:
	static void init();
	static void update();
	static float getTime();
	static float getDeltaTime();
private:
	static float lastTime, curTime, deltaTime;
};