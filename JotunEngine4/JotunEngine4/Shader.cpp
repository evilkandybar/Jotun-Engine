#include "Shader.h"


Shader::Shader(void) {}

Shader::Shader( GLuint newID ) {
	glid = newID;
}

Shader::Shader( std::string newName ) {
	name = newName;
}

Shader::~Shader(void) {}

void Shader::setID( GLuint newID ) {
	glid = newID;
}

void Shader::setName( std::string newName ) {
	name = newName;
}

void Shader::setActive() {
	glUseProgram( glid );
}

GLuint Shader::getID() {
	return glid;
}

std::string Shader::getName() {
	return name;
}