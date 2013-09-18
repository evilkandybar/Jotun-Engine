#include "Texture.h"

Texture::Texture(void) {}

Texture::Texture( std::string newName ) {
	data = stbi_load( newName.c_str(), &x, &y, &numChannels, 4 );
}

Texture::~Texture(void) {}