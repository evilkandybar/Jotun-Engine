#include "Shader.h"


Shader::Shader(void) {}

Shader::Shader( GLuint newID ) {
	setID( newID );
}

Shader::Shader( std::string newName ) {
	setName( newName );
}

Shader::~Shader(void) {}

void Shader::setID( GLuint newID ) {
	glid = newID;
	mvpPos = glGetUniformLocation( glid, "mvp" );
}

void Shader::setName( std::string newName ) {
	name = newName;
}

void Shader::bind() {
	glUseProgram( glid );
}

void Shader::uploadMatrix( glm::mat4 mvp ) {
	glUniformMatrix4fv( mvpPos, 1, GL_FALSE, &mvp[0][0] );
}

GLuint Shader::getID() {
	return glid;
}

std::string Shader::getName() {
	return name;
}