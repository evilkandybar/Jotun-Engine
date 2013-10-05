#include "Camera.h"

Camera::Camera() : position( 0, 0, 5 ) {
	init();
}

Camera::Camera( glm::vec3 initPos ) : position( initPos ) {
	init();
}

void Camera::init() {
	fov = 60;
	aspectRatio = .75f;
	verticalAngle = 0;
	horozontalAngle = 3.14159;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::setFOV( float newFOV ) {
	fov = newFOV;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::setAspectRatio( float newAspectRatio ) {
	aspectRatio = newAspectRatio;
	projMatrix = glm::perspective( fov, aspectRatio, 0.1f, 100.0f );
}

void Camera::lookAt( glm::vec3 pos ) {
	viewMatrix = glm::lookAt( position, pos, up );
}

void Camera::update() {
	//currently just calculates the forward, up, and right vectors and the relevant matricies
	forward = glm::vec3( cos( verticalAngle ) * sin( horizontalAngle ),
		sin( verticalAngle ), cos( verticalAngle ) * cos( horizontalAngle ) );

	right = glm::vec3( sin( horizontalAngle - 3.14f / 2.0f ),
		0, cos( horizontalAngle - 3.14f / 2.0f ) );
	
	up = glm::cross( right, forward );

	viewMatrix = glm::lookAt( position, position + forward, up );
}

float Camera::getFOV() {
	return fov;
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getForward() {
	return forward;
}

glm::vec3 Camera::getUp() {
	return up;
}

glm::vec3 Camera::getRight() {
	return right;
}

glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}
glm::mat4 Camera::getProjMatrix() {
	return projMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;


void computeMatricesFromInputs() {
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float( currentTime - lastTime );

	// Get mouse position
	int xpos, ypos;
	glfwGetMousePos( &xpos, &ypos );

	// Reset mouse position for next frame
	// EDIT : Doesn't work on Mac OS, hence the code below is a bit different from the website's
	//glfwSetMousePos(1024/2, 768/2);

	// Compute new orientation
	horizontalAngle = 3.14f + mouseSpeed * float( 1024 / 2 - xpos );
	verticalAngle = mouseSpeed * float( 768 / 2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos( verticalAngle ) * sin( horizontalAngle ),
		sin( verticalAngle ),
		cos( verticalAngle ) * cos( horizontalAngle )
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin( horizontalAngle - 3.14f / 2.0f ),
		0,
		cos( horizontalAngle - 3.14f / 2.0f )
		);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS ) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS ) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS ) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS ) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projMatrix = glm::perspective( FoV, 4.0f / 3.0f, 0.1f, 100.0f );
	// Camera matrix
	viewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}