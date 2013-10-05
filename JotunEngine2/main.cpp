#include "JotunEngine2.h"

#include <common/controls.hpp>

int		init();
void	initOpenGL();
void	initData();
int		main();

Camera *mainCamera, *lightCamera;

Shader *diffuse;
Shader *depth;
Shader *passthrough;

Mesh *mesh;

Texture *texture;

int init() {
	// Initialise GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint( GLFW_FSAA_SAMPLES, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 2 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 1 );

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW ) ) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW\n" );
		return -1;
	}

	glfwSetWindowTitle( "Tutorial 16" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwSetMousePos( 1024 / 2, 768 / 2 );
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

	lightCamera = new Camera( glm::vec3( 0.5f, 2, 2 ) );

	diffuse = new Shader( "Diffuse.vert", "Diffuse.frag" );
	depth = new Shader( "Depth.vert", "Depth.frag" );
	passthrough = new Shader( "Passthrough.vert", "Texture.frag" );
}

int main( void ) {
	if( init() == -1 ) {
		return(EXIT_FAILURE);
	}
	initOpenGL();
	initData();

	std::string *uniformNames = new std::string [] {"depthMVP"};
	std::string *attribNames = new std::string [] {"vertexPosition_modelspace"};

	depth->genUniformMap( uniformNames, 1 );
	depth->genAttribMap( attribNames, 1 );

	// Get a handle for our "MVP" uniform
	GLuint depthMatrixID = glGetUniformLocation(depth->getGLName(), "depthMVP");

	// Get a handle for our buffers
	GLuint depth_vertexPosition_modelspaceID = glGetAttribLocation( depth->getGLName(), "vertexPosition_modelspace" );

	// Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");

	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
		
	static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	uniformNames = new std::string [] { "texture" };
	attribNames = new std::string [] {"vertexPosition_modelspace"};

	passthrough->genAttribMap( attribNames, 1 );
	passthrough->genUniformMap( uniformNames, 1 );

	uniformNames = new std::string [] { "texture", "myTextureSampler", "MVP", "V", "M", 
		"DepthBiasMVP", "shadowMap", "LightInvDirection_modelspace" };
	attribNames = new std::string [] { "vertexPosition_modelspace", "vertexUV", 
		"vertexNormal_modelspace" };

	diffuse->genAttribMap( attribNames, 3 );
	diffuse->genUniformMap( uniformNames, 8 );

	do{
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
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
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
		// or, for spot light :
		//glm::vec3 lightPos(5, 20, 20);
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

		glm::mat4 depthModelMatrix = glm::mat4(1.0);
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
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteTextures(1, &Texture);

	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(1, &depthTexture);
	glDeleteBuffers(1, &quad_vertexbuffer);
	
	delete diffuse;
	delete passthrough;
	delete depth;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

