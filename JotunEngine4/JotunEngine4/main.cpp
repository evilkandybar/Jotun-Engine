#include "JotunEngine4.h"

void errorCallback( int, const char* );
int initWindow();
void initOpenGL();
void initEngine();
int main();
void draw();

//holds the handle for the main window
GLFWwindow *window;

MeshRenderer *mesh;

int initWindow() {
	glfwSetErrorCallback(errorCallback);
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW. Die.\n" );
		return -1;
	}
	glfwDefaultWindowHints();
	//glfwWindowHint( GLFW_MSAA_SAMPLES, 4 ); // 4x antialiasing
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // We want OpenGL 3.3
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
 
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 600, 400, "Jotun Engine 4", NULL, NULL );
	if( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window\n" );
	    glfwTerminate();
	    return -1;
	}

	glfwMakeContextCurrent( window );
 
	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW\n" );
	    return -1;
	}

	glfwSetWindowTitle( window, "Tutorial 01" );
}

void errorCallback(int err, const char* msg) {
	fprintf(stderr, "Error %d: %s\n", err, msg);
}

void initOpenGL() {
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LESS);
	glEnableVertexAttribArray(0);
}

void initEngine() {
	mesh = new MeshRenderer("HumanTransport.obj", "Diffuse");
}

void draw() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	mesh->draw();
	glfwSwapBuffers( window ); 
}

int main() {
	switch( initWindow() ) {
	case -1:
		system( "PAUSE" );
		return -1;
	}
	initOpenGL();
	initEngine();
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
 
	std::cout << "Starting main loop...\n";

	do {
		draw();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

    glfwTerminate();
    return 0;
}	