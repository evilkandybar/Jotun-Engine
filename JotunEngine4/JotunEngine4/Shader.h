#pragma once
#include "stdafx.h"

class Shader
{
public:
	Shader(const char *sName);
	Shader( std::string &sName );
	~Shader();
	int getGLID();
private:
	int loadShader(std::string &name);
	void validateShader(GLuint shader, const char *file = 0);
	GLuint glid;
	std::string name;
};

