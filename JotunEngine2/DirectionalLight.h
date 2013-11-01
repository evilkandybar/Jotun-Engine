#pragma once
#include "Light.h"
class DirectionalLight :
	public Light {
public:
	/*!\brief Pass these values to the Light constructor*/
	DirectionalLight( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition  );

	/*!\brief Presumably the Light destructor is called. Maybe not.*/
	~DirectionalLight();

	/*!\brief Renders the mesh using an orthogonal camera. As the light is assumed to 
		be at infinity, no culling happens
		
	\param [in] meshes The mesh to render
	\param [in] numMeshes The number of meshes to render
	\param [in] shader The shader to use in rendering
	\return The depthMVP matrix of the shadow, for purposes of getting shadow coordinates*/
	virtual glm::mat4 renderShadow( Mesh *meshes, int numMeshes, Shader *shader );

	/*!\brief Returns a vec4 with the light's position (assumed to be its direction) in the 
		xyz and 0 in the w*/
	virtual glm::vec4 getPos();
};

