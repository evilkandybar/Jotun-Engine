#pragma once
#include "stdafx.h"
/*!\brief Defines a 2D texture object for use in rendering or computation*/
class Texture {
public:
	/*!\brief Constructs a texture, loading the data from the specified file
	
	\param [in] name The filename of the texture to load*/
	Texture( std::string name );
	/*!\brief Default destructor*/
	~Texture();
	/*!\brief Binds this texture to the specified texture unit
	
	\param [in] unit The texture unit to bind this texture to. Must be between 0 and 7 inclusive*/
	void		bind( int unit );
	/*!\brief Returns the integer OpenGL uses to refer to this texture
	
	\return The integer OpenGL uses to refer to this texture*/
	GLuint		getGLName() { return glName; };
private:
	void		loadTexture();
	void		makeGLTex();
	int			width, height, comp;
	std::string sName;
	GLubyte		*data;
	GLuint		glName;
};

