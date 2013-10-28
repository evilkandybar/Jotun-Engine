#pragma once
/*\brief A simple state machine to hold the current input buttons in a place
easily accessable from anywhere inside the program*/
class Input {
public:
	/*!\brief Clears the input buffers to prepare for writing*/
	static void clear();
	/*!\brief Sets the mouse position and change in mouse position
	
	\param [in] newMouseX The current x-position of the mouse, in screen 
		coordinates
	\param [in] newMouseY The current y-position of the mouse, in screen
		coordinates*/
	static void updateMouse( double newMouseX, double newMouseY );

	/*!\brief An array of boolean flags, one for each key defined by GLFW
	
	See http://www.glfw.org/docs/3.0/group__keys.html for a complete listing*/
	static bool keys [349];
	/*\brief Variables for the x-position, y-position, x change, and y change
	of the mouse*/
	static double mouseX, mouseY, mouseDeltaX, mouseDeltaY;
};

