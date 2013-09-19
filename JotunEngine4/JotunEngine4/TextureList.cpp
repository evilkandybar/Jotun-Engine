#include "TextureList.h"

std::vector<Texture*> TextureList::textures;

Texture* TextureList::loadTexture( std::string name ) {
	return new Texture();
}