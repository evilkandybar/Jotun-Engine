#pragma once
#include "stdafx.h"
#include "Mesh.h"
class MeshLoader {
public:
	/*!\brief Searches for a mesh with the specified name. If no such mesh
		exists, it loads the mesh from disk. The desired mesh is returned
		
	\param [in] mName The name of the mesh the user wishes to access
	\return The mesh the user wishes to access*/
	static Mesh *getMesh( std::string mName );

	/*!\brief Deletes all meshes, freeing up their RAM*/
	static void clearAll();
private:
	static void loadMesh( std::string mName );
	static int binsearch( std::string toFind );
	static int binsearch( int start, int end, std::string toFind );
	static std::vector<Mesh*> meshes;
};