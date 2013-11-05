#include "Framebuffer.h"


Framebuffer::Framebuffer( int newWidth, int newHeight ) {
	width = newWidth;
	height = newHeight;

#pragma region buffer
	glGenFramebuffers( 1, &buffer );
	glBindFramebuffer( GL_FRAMEBUFFER, buffer );
#pragma endregion
#pragma region depth
	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures( 1, &depth );
	glBindTexture( GL_TEXTURE_2D, depth );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depth, 0 );
#pragma endregion
#pragma region color;
	// Color texture, because post-processing
	glGenTextures( 1, &color );
	glBindTexture( GL_TEXTURE_2D, color );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, color, 0 );
#pragma endregion
#pragma region stencil
	// Stencil texture. This is probably useful maybe
	glGenTextures( 1, &stencil );
	glBindTexture( GL_TEXTURE_2D, stencil );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, stencil, 0 );
#pragma endregion
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers( 1, &buffer );
	glDeleteTextures( 1, &depth );
	glDeleteTextures( 1, &color );
	glDeleteTextures( 1, &stencil );
}

void Framebuffer::bind() {
	glBindFramebuffer( GL_FRAMEBUFFER, buffer );
}

GLuint Framebuffer::getWidth() {
	return width;
}

GLuint Framebuffer::getHeight() {
	return height;
}

GLuint Framebuffer::getBuffer() {
	return buffer;
}

GLuint Framebuffer::getColorTexture() {
	return color;
}

GLuint Framebuffer::getDepthTexture() {
	return depth;
}

GLuint Framebuffer::getStencilTexture() {
	return stencil;
}