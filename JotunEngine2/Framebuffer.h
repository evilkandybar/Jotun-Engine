#pragma once
#include "stdafx.h"
class Framebuffer {
public:
	/*!\brief Constructs a new framebuffer with the given width and height
	
	\param [in] newWidth The desired width of the framebuffer
	\param [in] newHeight The desired height of the framebuffer*/
	Framebuffer( int newWidth, int newHeight );

	/*!\brief Default destructor*/
	~Framebuffer();

	/*!\brief Binds this framebuffer so we can render to it*/
	void bind();

	/*!\brief Returns the width of this framebuffer
	
	\return The width of this framebuffer*/
	GLuint getWidth();

	/*!\brief Returns the height of this framebuffer
	
	\return The height of this framebuffer*/
	GLuint getHeight();

	/*!\brief Returns the OpenGL name for this framebuffer
	
	\return The OpenGL name for this framebuffer*/
	GLuint getBuffer();

	/*!\brief Returns the OpenGL name for the color texture attached to this framebuffer
	
	\return The OpenGL name for the color texture*/
	GLuint getColorTexture();

	/*!\brief Returns the OpenGL name for the depth texture attached to this framebuffer
	
	\return The OpenGL name for the depth texture*/
	GLuint getDepthTexture();

	/*!\brief Returns the OpenGL name for the stencil texture attached to the framebuffer
	
	\return The OpenGL name for the stencil buffer*/
	GLuint getStencilTexture();
private:
	GLuint buffer, depth, color, stencil;
	int width, height;
};

