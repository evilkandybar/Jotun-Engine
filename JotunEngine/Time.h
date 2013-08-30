#pragma once
class Time
{
public:
	static void init() {lastClock = 0; time = 0; deltaTime = 0;};
	static void update();
	static double time;
	static double deltaTime;
	static double framerate;
protected:
	static double lastClock;
};

