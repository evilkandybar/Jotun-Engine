#include "stdafx.h"

#include "Mesh.h"
#include "Shader.h"
#include "ShaderList.h"

int init();
int main();

//holds the handle for the main window
GLFWwindow *window;

Mesh *mesh;
Shader *shader;

int init() {
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW. Die.\n" );
		return -1;
	}
	glfwDefaultWindowHints();
	//glfwWindowHint( GLFW_MSAA_SAMPLES, 4 ); // 4x antialiasing
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 ); // We want OpenGL 4.3
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	//glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); //We don't want the old OpenGL
 
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 600, 400, "Jotun Engine 4", NULL, NULL );
	if( window != NULL )
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
	mesh = new Mesh();
	shader = ShaderList::loadShader( "Diffuse" );
}

int main() {
	switch( init() ) {
	case -1:
		system( "PAUSE" );
		return -1;
	}
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
 
	while( true ) {//glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS ) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		shader->setActive();
		mesh->draw();
		glfwSwapBuffers( window ); 
	}

    glfwTerminate();
    return 0;
}	