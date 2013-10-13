#pragma once
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
/*!\brief Currently just handles movement. Will be expanded to rotations*/
class Transformable {
public:
	virtual void		updateTrans() = 0;
	virtual void		translate( glm::vec3 dir ) = 0;
	virtual glm::vec3	getPosition() = 0;
	virtual glm::vec3	getForward() = 0;
	virtual glm::vec3   getUp() = 0;
	virtual glm::vec3	getRight() = 0;
protected:
	glm::vec3 position, up, right, forward;
};

