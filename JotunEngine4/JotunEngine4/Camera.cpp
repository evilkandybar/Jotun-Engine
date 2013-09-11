#include "Camera.h"

Camera::Camera(void) {
	projection = glm::perspective<float>( 60, float(4)/float(3), 0.1, 100 );
	view = glm::lookAt( 
		glm::vec3( 10, 10, 10 ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 ) );
}

Camera::~Camera(void) {}

glm::mat4 Camera::transformModel( glm::mat4 &model ) {
	return projection * view * model;
}