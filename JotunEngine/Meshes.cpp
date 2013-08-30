#include "stdafx.h"
#include "Meshes.h"

std::vector<Mesh*> Meshes::meshes;

Mesh* Meshes::loadMesh( std::string name ) {
	Mesh *mesh = getMesh( name );
	if( mesh != NULL ) {
		return mesh;
	}
	mesh = new Mesh( name );
	meshes.push_back( mesh );
	return mesh;
}

Mesh* Meshes::getMesh( std::string name ) {
	for( Mesh *m : meshes ) {
		if( m->getName() == name ) {
			return m;
		}
	}
	return new Mesh( name );
}