#include "MeshLoader.h"

std::vector<Mesh*> MeshLoader::meshes;

void MeshLoader::loadMesh( std::string mName ) {
	Mesh *m = new Mesh( mName );
	meshes.push_back( m );
}

void MeshLoader::clearAll() {
	for( Mesh *m : meshes ) {
		delete m;
	}
}

Mesh *MeshLoader::getMesh( std::string mName ) {
	int pos = binsearch( mName );
	Mesh *m;
	if( pos == -1 ) {
		m = new Mesh( mName );
		meshes.push_back( m );
	}
	m = meshes[pos];
	return m;
}

int MeshLoader::binsearch( std::string toFind ) {
	return binsearch( 0, meshes.size(), toFind );
}

int MeshLoader::binsearch( int start, int end, std::string toFind ) {
	if( end <= start ) {
		return -1;
	}
	int midPos = (end - start) / 2;
	int val = meshes[midPos]->getName().compare( toFind );
	if( val == 0 ) {
		return midPos;
	} else if( val < 0 ) {
		start = midPos + 1;
	} else if( val > 0 ) {
		end = midPos - 1;
	}
	return binsearch( start, end, toFind );
}