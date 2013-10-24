#pragma once
#include "stdafx.h"

class Shader {
public:
				Shader( std::string vertName, std::string fragName );
				~Shader();
	void		genAttribMap( std::string varNames [], int numElems );
	void		genUniformMap( std::string varNames [], int numElems );
	void		setUniform1i( std::string name, int v1 );
	void		setUniform2i( std::string name, int v1, int v2 );
	void		setUniform3f( std::string name, int v1, int v2, int v3 );
	void		setUniformMat4x4( std::string name, float* value );
	void		bind();
	GLuint		getAttribute( std::string attrib );
	GLuint		getGLName() { return glName; };
	std::string getSName() { return sName; };
private:
	void		loadShader( const char * vertFile, const char * fragFile );

	std::map<std::string, int> attribs;
	std::map<std::string, int> uniforms;
	std::string sName;
	GLuint		glName;
};