#include "stdafx.h"
#include "Shaders.h"

std::vector<Shader*> Shaders::shaders;

int Shaders::loadShader( std::string name ) {
	int ret = getShaderByName( name )->id();
	if( ret != -1 ) {
		return ret;
	}
	std::string vName = name;
	vName += ".vert";
	name += ".frag";
	Shader *s = new Shader( name, vName.c_str(), name.c_str() );
	shaders.push_back( s );
	return shaders[shaders.size()-1]->id();
}

Shader* Shaders::getShaderByIndex( int ind ) {
	if( ind < -1 || ind > shaders.size() ) {
	Shader *s = new Shader( -1 );
		return s;
	}
	return shaders[ind];
}

Shader* Shaders::getShaderByName( std::string name ) {
	int len = shaders.size();
	for( Shader *s : shaders ) {
		if( s->getName() == name ) {
			return s;
		}
	}
	Shader *s = new Shader( -1 );
	return s;
}

Shader* Shaders::getShaderByGLID( int glid ) {
	for( Shader *s : shaders ) {
		if( s->id() == glid ) {
			return s;
		}
	}
	Shader *s = new Shader( -1 );
	return s;
}