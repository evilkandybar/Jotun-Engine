#include "stdafx.h"
#include "Shader.h"
#pragma once
class ShaderList
{
public:
	ShaderList(void);
	~ShaderList(void);
	static Shader* loadShader( std::string name );
};

