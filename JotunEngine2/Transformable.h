#pragma once
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
/*!\brief Defines an interface movable objects can inherit from

Currently just handles movement. Will be expanded to rotations*/
class Transformable {
public:
	/*!\brief Updates the values inherited from Transformable, calculating the forward, up, and right
	vectors
	
	This function is virtual so that the user may define a wat to calculate the forward, up, and
	right vectors*/
	virtual void		updateTrans() = 0;
	/*!\brief Moves this Transformable by the specified vector
	
	\param [in] dir The amount to move this Transformable by*/
	void		translate( glm::vec3 dir ) { position += dir; };
	/*!\brief Returns the current position of this Transformable
	
	\return The current position of this Transformable*/
	glm::vec3	getPosition() { return position; };
	/*!\brief Returns the forward vector of this Transformable
	
	\return The forward vector of this transformable*/
	glm::vec3	getForward() { return forward; };
	/*!\brief Returns the up vector of this Transformable
	
	\return The up vector of this Transformable*/
	glm::vec3   getUp() { return up; };
	/*!\brief Returns the right vector of this Transformable
	
	\return The right vector of this Transformable*/
	glm::vec3	getRight() { return right; };
protected:
	glm::vec3 position, up, right, forward;
};

