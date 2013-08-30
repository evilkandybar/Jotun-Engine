#pragma once

#include "stdafx.h"
#include "Vector3.h"

class Mesh
{
public:
    Mesh();
	Mesh( std::string );
	~Mesh();

    bool						import( std::string );
    void						render();
	void						renderNode( const aiScene*, const aiNode* );

	Vector3&					getVert( int i, Vector3* );		//the caller must allocate some memory so the memory 
	Vector3&					getNormal( int i, Vector3* );	//won't soon go out of scope
	std::string&				getName() {return name;};

private:
	char						*shaderPath;
	 
	int							lastPos, fVertsSize, fNormsSize, iFacesSize, iFaceNormsSize, fTexCordsSize;
	int							shader;

	bool						firstRender;
		
	std::string					name;
	std::ofstream				impOut;

	const aiScene				*scene;
};