#pragma once
class Input {
public:
	static void clear();
	static void updateMouse( double newMouseX, double newMouseY );
	static bool keys [349];
	static double mouseX, mouseY, mouseDeltaX, mouseDeltaY;
};

