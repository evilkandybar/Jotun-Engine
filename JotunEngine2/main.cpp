#include "JotunEngine2.h"

int		init();
void	initOpenGL();
void	initData();
int		main();
void	draw();
void	destroy();
void	onGLFWError( int error, const char *description );
void	onGLFWKey( GLFWwindow *curWindow, int key, int scancode, int action, int mods );

std::vector<InputHandler*> inputHandlers;

GLFWwindow *window;

Camera *mainCamera, *lightCamera;

Shader *diffuse;
Shader *depth;
Shader *passthrough;

Mesh *mesh;

Texture *texture;

GLuint depthTexture;
GLuint FramebufferName = 0;

int init() {
	glfwSetErrorCallback( onGLFWError );
	// Initialise GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint( GLFW_SAMPLES, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 786, "Jotun Engine 2", NULL, NULL );
	if( !window ) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, onGLFWKey );
	// Initialize GLEW
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW\n" );
		return -1;
	}

	glfwSetWindowTitle( window, "Tutorial 16" );

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
	glfwSetCursorPos( window, 1024 / 2, 768 / 2 );
}

void initOpenGL() {
	// Dark blue background
	glClearColor( 0.0f, 0.0f, 0.4f, 0.0f );

	// Enable depth test
	glEnable( GL_DEPTH_TEST );

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc( GL_LESS );

	// Cull triangles which normal is not towards the camera
	glEnable( GL_CULL_FACE );
}

void initData() {
	// Read our .obj file
	mesh = new Mesh( "room_thickwalls.obj" );

	mainCamera = new Camera();

	inputHandlers.push_back( mainCamera );

	lightCamera = new Camera( glm::vec3( 0.5f, 2, 2 ) );

	diffuse = new Shader( "Diffuse.vert", "Diffuse.frag" );
	depth = new Shader( "Depth.vert", "Depth.frag" );
	passthrough = new Shader( "Passthrough.vert", "Texture.frag" );
	Time::init();
}

void draw() {// Render to our framebuffer
	glBindFramebuffer( GL_FRAMEBUFFER, FramebufferName );
	glViewport( 0, 0, 1024, 1024 ); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	// We don't use bias in the shader, but instead we draw back faces, 
	// which are already separated from the front faces by a small distance 
	// (if your geometry is made this way)
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK ); // Cull back-facing triangles -> draw only front-facing triangles

	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Use our shader
	depth->bind();

	glm::vec3 lightInvDir = glm::vec3( 0.5f, 2, 2 );

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>( -10, 10, -10, 10, -10, 20 );
	glm::mat4 depthViewMatrix = glm::lookAt( lightInvDir, glm::vec3( 0, 0, 0 ), glm::vec3( 0, 1, 0 ) );
	// or, for spot light :
	//glm::vec3 lightPos(5, 20, 20);
	//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

	glm::mat4 depthModelMatrix = glm::mat4( 1.0 );
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	depth->setUniformMat4x4( "depthMVP", &depthMVP[0][0] );

	mesh->drawShadowPass( depth->getAttribute( "vertexPositino_modelspace" ) );

	// Render to the screen
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, 1024, 768 );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Use our shader
	diffuse->bind();

	mainCamera->update();
	glm::mat4 ProjectionMatrix = mainCamera->getProjMatrix();
	glm::mat4 ViewMatrix = mainCamera->getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4( 1.0 );
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	glm::mat4 depthBiasMVP = biasMatrix*depthMVP;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	diffuse->setUniformMat4x4( "MVP", &MVP[0][0] );
	diffuse->setUniformMat4x4( "M", &ModelMatrix[0][0] );
	diffuse->setUniformMat4x4( "V", &ViewMatrix[0][0] );
	diffuse->setUniformMat4x4( "DepthBiasMVP", &depthBiasMVP[0][0] );

	diffuse->setUniform3f( "LightInvDirection_modelspace",
		lightInvDir.x, lightInvDir.y, lightInvDir.z );

	// Bind our texture in Texture Unit 0
	texture->bind( 0 );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	diffuse->setUniform1i( "myTextureSampler", 0 );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, depthTexture );
	diffuse->setUniform1i( "shadowMap", 1 );

	mesh->enable( diffuse->getAttribute( "vertexPosition_modelspace" ),
		diffuse->getAttribute( "vertexUV" ),
		diffuse->getAttribute( "vertexNormals_modelspace" ) );
	mesh->bind();
	mesh->draw();
	mesh->disable();

	// Swap buffers
	glfwSwapBuffers( window );
}

int main( void ) {
	if( init() == -1 ) {
		return EXIT_FAILURE;
	}
	initOpenGL();
	initData();

	std::string *uniformNames = new std::string[1]; 
	uniformNames[0] = "depthMVP";
	std::string *attribNames = new std::string[1];
	attribNames[0] = "vertexPosition_modelspace";

	depth->genUniformMap( uniformNames, 1 );
	depth->genAttribMap( attribNames, 1 );

	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers( 1, &FramebufferName );
	glBindFramebuffer( GL_FRAMEBUFFER, FramebufferName );

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures( 1, &depthTexture );
	glBindTexture( GL_TEXTURE_2D, depthTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0 );

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );

	// Always check that our framebuffer is ok
	GLuint error = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {
		return false;
	}

	uniformNames = new std::string[1];
	uniformNames[0] = "texture";
	attribNames = new std::string[1];
	attribNames[0] = "vertexPosition_modelspace";

	passthrough->genAttribMap( attribNames, 1 );
	passthrough->genUniformMap( uniformNames, 1 );

	uniformNames = new std::string[8];
	uniformNames[0] = "texture";
	uniformNames[1] = "myTextureSampler";
	uniformNames[2] = "MVP";
	uniformNames[3] = "V";
	uniformNames[4] = "M";
	uniformNames[5] = "DepthBiasMVP";
	uniformNames[6] = "shadowMap"; 
	uniformNames[7] = "LightInvDirection_modelspace";

	attribNames = new std::string[3];
	attribNames[0] = "vertexPosition_modelspace";
	attribNames[1] = "vertexUV";
	attribNames[2] = "vertexNormal_modelspace";

	diffuse->genAttribMap( attribNames, 3 );
	diffuse->genUniformMap( uniformNames, 8 );

	while( !glfwWindowShouldClose( window ) ) {
		Time::update();
		draw();
		glfwPollEvents();
	}
	destroy();
	return 0;
}

void destroy() {
	glDeleteFramebuffers( 1, &FramebufferName );
	glDeleteTextures( 1, &depthTexture );
	// Close OpenGL window and terminate GLFW
	glfwTerminate(); 

	delete mainCamera;
	delete lightCamera;
	delete diffuse;
	delete depth;
	delete passthrough;
	delete mesh;
	delete texture;
}

static void onGLFWError( int error, const char *description ) {
	printf( "Error %d: %s\n", error, description );
}

static void onGLFWKey( GLFWwindow *curWindow, int key, int scancode, int action, int mods ) {
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
	if( action == GLFW_PRESS ) {
		for( InputHandler *ih : inputHandlers ) {
			ih->onKeyPress( key );
		}
	} else if( action == GLFW_RELEASE ) {
		for( InputHandler *ih : inputHandlers ) {
			ih->onKeyRelease( key );
		}
	}
}