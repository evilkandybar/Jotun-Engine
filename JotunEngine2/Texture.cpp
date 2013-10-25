#include "Texture.h"

Texture::Texture( std::string sName ) {
	width = 0;
	this->sName = sName;
	loadTexture();
	if( width == 0 ) {
		std::cout << "Texture load failed. Ignore all consecutive anythings\n";
		return;
	}
	makeGLTex();
}

Texture::~Texture() {
	delete [] data;
}

void Texture::loadTexture() {
	std::cout << "Loading texture " << sName << "...\n";
	data = stbi_load( sName.c_str(), &width, &height, &comp, 0 );
}

void Texture::makeGLTex() {
	glGenTextures( 1, &glName );
	bind( 0 );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// Nice trilinear filtering.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glGenerateMipmap( GL_TEXTURE_2D );

	if( comp == 3 ) {	//which it might. Maybe. Once or twice...
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
	} else if( comp == 4 ) {	//Everything needs an alpha channel
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
	}
}

void Texture::bind( int unit ) {
	glActiveTexture( GL_TEXTURE0 + unit );
	glBindTexture( GL_TEXTURE_2D, glName );
}

//everblack