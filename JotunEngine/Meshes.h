#pragma once

#include "Mesh.h"

//A container class for all the meshes in the scene. This is so I can easily use the same mesh multiple times
//Each RenderMesh contains a reference to a mesh maintained here
class Meshes
{
public:
	static Mesh* loadMesh( std::string name );
	static Mesh* getMesh( int ind ) {return meshes[ind];};
	static Mesh* getMesh( std::string name );
protected:
	static std::vector<Mesh*> meshes;
};