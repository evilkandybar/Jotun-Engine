#include "stdafx.h"

#include "Mesh.h"
#include "Shader.h"
#include "ShaderList.h"

void errorCallback( int, const char* );
int init();
void initOpenGL();
int main();
void draw();

//holds the handle for the main window
GLFWwindow *window;

Mesh *mesh;
Shader *shader;

int init() {
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

	glEnableVertexAttribArray(0);
	glfwSetWindowTitle( window, "Tutorial 01" );
	initOpenGL();
	mesh = new Mesh();
	shader = ShaderList::loadShader( "Diffuse" );
}

void errorCallback(int err, const char* msg) {
	fprintf(stderr, "Error %d: %s\n", err, msg);
}

void initOpenGL() {
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
}

void draw() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	shader->setActive();
	mesh->draw();
	glfwSwapBuffers( window ); 
}

int main() {
	switch( init() ) {
	case -1:
		system( "PAUSE" );
		return -1;
	}
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
 
	std::cout << "Starting main loop...\n";

	do {
		draw();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

    glfwTerminate();
    return 0;
}	