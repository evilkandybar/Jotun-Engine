#pragma once
#include "Color.h"
#include "Shader.h"
#include "Shaders.h"
class Material
{
public:
	Material(void);
	Material( std::string shaderName );
	~Material(void);
protected:
	Color diffuse;
	float specularPower;
	Shader *shader;
};