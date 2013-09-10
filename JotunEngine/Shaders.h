#pragma once

#include "Shader.h"

class Shaders
{
public:
	static void init();
	static int loadShader( std::string name );
	static Shader* getShaderByIndex( int ind );
	static Shader* getShaderByName( std::string name );
	static Shader* getShaderByGLID( int glid );
private:
	static std::vector<Shader*> shaders;
};