#pragma once
#include "stdafx.h"
#include "Texture.h"
class TextureList
{
public:
	static Texture* loadTexture( std::string name );
private:
	static std::vector<Texture*> textures;
};