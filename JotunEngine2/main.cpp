#include "JotunEngine2.h"
//********//********//********//********//********//********//********//********
//#include <common/controls.hpp>

int		init();
void	initOpenGL();
void	initData();
int		main();
void	draw();
void	destroy();
void	onGLFWError( int error, const char *description );
void	onGLFWKey( GLFWwindow *curWindow, int key, int scancode, 
			int action, int mods );
void	onGLFWMouse( GLFWwindow *window, double xPos, double yPos );

int width, height;

std::vector<InputHandler*> inputHandlers;

GLFWwindow *window;

Camera *mainCamera;

DirectionalLight *mainLight;

Framebuffer **lastEightFrames;
char curFrame;	//from 0 to 7

Shader *diffuse;
Shader *internalLighting;
Shader *depth;
Shader *motionBlur;
Shader *passthrough;
Shader *vertexLit;

Mesh *mesh;

Texture *texture;
Texture *normalMap;

int init() {
	width = 1024;
	height = 786;
	glfwSetErrorCallback( onGLFWError );
	// Initialise GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	
	glfwWindowHint( GLFW_SAMPLES, Settings::getMultisamples() );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( width, height, "Jotun Engine 2", NULL, NULL );
	if( !window ) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, onGLFWKey );
	glfwSetCursorPosCallback( window, onGLFWMouse );

	// Initialize GLEW
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW\n" );
		return -1;
	}

	glfwSetWindowTitle( window, "Tutorial 16" );

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
	glfwSetCursorPos( window, width / 2, height / 2 );
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
	glCullFace( GL_BACK );

	glEnable( GL_TEXTURE_2D );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void initData() {
	//initialize eight framebuffers for use in motion blur
	for( int i = 0; i < 8; i++ ) {
		lastEightFrames[8] = new Framebuffer( width, height );
	}
	curFrame = 0;

	// Read our .obj file
	mesh = new Mesh( "room_thickwalls.obj" );

	mainCamera = new Camera( glm::vec3( 7.48113, 6.50764, 5.34367 ) );

	inputHandlers.push_back( mainCamera );
	
	mainLight = new DirectionalLight( glm::vec3( 1, 1, 0 ), 1, glm::vec3( 7.5f, 2, 2 ) );

	diffuse = new Shader( "Diffuse.vert", "Diffuse.frag" );
	depth = new Shader( "Depth.vert", "Depth.frag" );
	motionBlur = new Shader( "Passthrough.vert", "MotionBLur.frag" );
	passthrough = new Shader( "Passthrough.vert", "Texture.frag" );
	vertexLit = new Shader( "VertexLit.vert", "VertexLit.frag" );
	internalLighting = new Shader( "InternalLighting.vert", "InternalLighting.frag" );

	texture = new Texture( "DiffuseTex.png" );
	normalMap = new Texture( "NormalMap.png" );
	Time::init();
}

void draw() {
#pragma region shadow
	glm::mat4 depthMVP = glm::mat4( 1.0 );
	glm::vec4 lightInvDir = mainLight->getPos();

	//Only render the shadow pass if we have shadows enabled
	if( Settings::getShadowQuality() > 0 ) {
		//We'll give the meshes and shaders and whatnot to the light so it can render them however
		//the light returns the depthMVP
		depthMVP = mainLight->renderShadow( mesh, 1, depth );
	}
#pragma endregion

#pragma region main_render
	// Render to the screen
	lastEightFrames[curFrame]->bind();
	curFrame++;
	if( curFrame > 7 ) {
		curFrame = 0;
	}
	glViewport( 0, 0, width, height );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	mainCamera->update();
	glm::mat4 ProjectionMatrix = mainCamera->getProjMatrix();
	glm::mat4 ViewMatrix = mainCamera->getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4( 1.0 );	//change this per model
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	// Use our shader
	diffuse->bind();

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	diffuse->setUniformMat4x4( "MVP", &MVP[0][0] );
	diffuse->setUniformMat4x4( "M", &ModelMatrix[0][0] );
	diffuse->setUniformMat4x4( "V", &ViewMatrix[0][0] );
	diffuse->setUniformMat4x4( "DepthBiasMVP", &depthBiasMVP[0][0] );

	diffuse->setUniform4f( "lightPosition_worldspace",
		lightInvDir.x, lightInvDir.y, lightInvDir.z, lightInvDir.w );

	diffuse->setUniform4f( "lightColor", 
		mainLight->getColor().r, mainLight->getColor().g, 
		mainLight->getColor().b, mainLight->getColor().a );

	// Bind our texture in Texture Unit 0
	normalMap->bind( 0 );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	diffuse->setUniform1i( "diffuse", 0 );

	normalMap->bind( 1 );
	diffuse->setUniform1i( "normalMap", 1 );

	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, mainLight->getDepthTexture() );
	diffuse->setUniform1i( "shadowMap", 2 );

	diffuse->setUniform1i( "shadowLevel", Settings::getShadowQuality() );

	internalLighting->bind();
	internalLighting->setUniformMat4x4( "MVP", &MVP[0][0] );
	internalLighting->setUniformMat4x4( "M", &ModelMatrix[0][0] );
	internalLighting->setUniformMat4x4( "V", &ViewMatrix[0][0] );

	mesh->bind( internalLighting->getAttribute( "vertexPosition_worldspace" ),
		internalLighting->getAttribute( "vertexUV" ),
		internalLighting->getAttribute( "vertexNormal_modelspace" ),
		internalLighting->getAttribute( "vertexTangent_modelspace" ) );
	mesh->draw();
#pragma endregion

#pragma region debugRenders
#pragma endregion

	glfwSwapBuffers( window );
}

int main( void ) {
	if( init() == -1 ) {
		return EXIT_FAILURE;
	}
	initOpenGL();
	initData();

	while( !glfwWindowShouldClose( window ) ) {
		Time::update();
		mainCamera->update();
		mainLight->update();
		draw();
		glfwPollEvents();
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	delete mainCamera;
	delete mainLight;
	delete diffuse;
	delete depth;
	delete passthrough;
	delete mesh;
	delete texture;

	MeshLoader::clearAll();

	return EXIT_SUCCESS;
}

void destroy() {
	// Close OpenGL window and terminate GLFW
	glfwTerminate(); 

	delete mainCamera;
	delete mainLight;
	delete diffuse;
	delete depth;
	delete passthrough;
	delete mesh;
	delete texture;
}

void onGLFWError( int error, const char *description ) {
	printf( "Error %d: %s\n", error, description );
}

void onGLFWKey( GLFWwindow *curWindow, int key, int scancode, 
	int action, int mods ) {
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
	if( action == GLFW_PRESS ) {
		Input::keys[key] = true;
		for( InputHandler *ih : inputHandlers ) {
			ih->onKeyPress( key );
		}
	} else if( action == GLFW_RELEASE ) {
		Input::keys[key] = false;
		for( InputHandler *ih : inputHandlers ) {
			ih->onKeyRelease( key );
		}
	}
}

static void onGLFWMouse( GLFWwindow *window, double xPos, double yPos ) {
	Input::updateMouse( xPos, yPos );
	for( InputHandler *ih : inputHandlers ) {
		ih->onMouseMove( Input::mouseX, Input::mouseY );
	}
}
