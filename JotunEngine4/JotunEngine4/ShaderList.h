#include "stdafx.h"
#include "Shader.h"
#pragma once
class ShaderList
{
public:
	static Shader* loadShader( std::string name );
private:
	static std::vector<Shader*> shaders;
};

