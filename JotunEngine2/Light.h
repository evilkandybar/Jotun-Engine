#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
//********//********//********//********//********//********//********//********
/*!\brief Defines light to be used in rendering

This class contains a camera for shadowing, a position, a color, and in intensity
In shaders, these values are combined with the shader's color, and reflectivity
This class contains n ospecularity information as that should bt handled per
material*/
class Light {
public:
	/*!\brief Default constructor. Initializes every value to zero*/
	Light();

	/*!\brief Constructs a Light with the specified position, color, and 
		intensiy
	
	\param [in] color The desired color of this light
	\param [in] intensity The intensity of this light, in lumens
	\param [in] position The initial position of this light*/
	Light( glm::vec3 &newColor, GLfloat intensity, glm::vec3 &newPosition );

	/*!\default destructor*/
	~Light();

	/*!\brief Updates the light, aligning the camera with the light*/
	void update();

	/*!\brief Renders the speficied mesh onto the framebuffer using the specified shader. Note that
		rendering happens differently for different sorts of lights
	
	\param [in] meshes The array of meshes to render
	\param [in] numMeshes The number of meshes to render
	\param [in] shader The shader to use (it's always the same, grab from shader list in constructor?)
	\return The depthMVP matrix of the shadow, for purposes of getting shadow coordinates*/
	virtual glm::mat4 renderShadow( Mesh *meshes, int numMeshes, Shader *shader ) = 0;

	/*!\brief Sets the color of the light to a specified value
	
	\param [in] newColor The desired new color of the light*/
	void setColor( glm::vec3 &newColor );

	/*!\brief Sets the intensity of the light

	\param [in] intensity The desired intensity of the light*/
	void setIntensity( GLfloat intensity );

	/*!\brief Sets the position of the light
	
	\param [in] newPos The desired position of this light*/
	void setPosition( glm::vec3 &newPos );

	/*!\brief Returns the OpenGL name for the depth texture, for use in shadowing
	
	\return The OpenGL name for the depth texture, for use in shadowing*/
	GLuint getDepthTexture();

	/*!\brief Returns a view matrix for a camera looking from the light to the
	specified position
	
	\param [in] screenCenter The place for the camera to look. This should be 
	a little way in front the the main camera
	\return The view matrix for the light's shadow camera*/
	glm::mat4 &getViewMatrix( glm::vec3 &screenCenter );

	/*!\brief Returns a projection matrix for the light's camera, used for the
	purposes of shadow mapping*/
	glm::mat4 &getProjMatrix();

	/*!\brief Returns the position of the light
	
	\return The position of the light, plus a number telling us what kind of light we have*/
	virtual glm::vec4 getPos() = 0;

	/*!\brief Returns the color and intensity of the light
	
	\return The color and intensity of the light. The RGB color is in the
	returned vector4's RGB, respectively. The intensity of the light is packed 
	into the returned Vector4's alpha*/
	glm::vec4 &getColor();
protected:
	GLuint framebufferName, depthTexture;
	glm::vec4 color;	//a is the light's intensity
	glm::vec3 position;
	Camera *shadowCam;
};

