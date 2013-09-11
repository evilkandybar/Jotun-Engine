#include "Texture.h"

Texture::Texture(void) {}

Texture::Texture( std::string newName ) {
	FILE *texFile = fopen( newName.c_str(), "r" );
	if( texFile != NULL ) {
		data = stbi_load_from_file( texFile, &x, &y, &numChannels, 4 );
	}
	fclose( texFile );
}

Texture::~Texture(void) {}