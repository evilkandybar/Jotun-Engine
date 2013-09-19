#pragma once
#include "stdafx.h"

/*!\brief A simple helper class*/

/*!\brief Stores aiMesh pointers for use in MeshRenderers
*/
class MeshList
{
public:
	static aiMesh*  loadMesh(std::string mName);
private:
	static std::map<std::string, aiMesh*> meshes;
};

