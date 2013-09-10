#pragma once
//Shader.h and Shader.cpp taken from http://www.swiftless.com/tutorials/glsl/7_texturing.html

#include "stdafx.h"
#include "Vector3.h"

#define SHADER_PATH "C:/Users/David/Documents/AMHS/2012-2013/AP Computer Science/AGameWithGod/res/shaders/"

class Shader {
public:
	Shader();
	Shader( int id ) {shader_id = id;};
	Shader( std::string name, const char *vsFile, const char *fsFile );
	~Shader();
	
    void init( const char *vsFile, const char *fsFile );
    
	void bind();
	void unbind();

	void setVariable( std::string, Vector3 );
	void setVariable( std::string, float );
	void setVariable( std::string, float, float, float, float );
	void setVariable( std::string, int );
	
	unsigned int id();
	std::string getName() {return strName;};

	static void validateProgram(GLuint);
	static void validateShader(GLuint, const char*);

private:
	std::string strName;
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
	static std::ofstream debug;
};