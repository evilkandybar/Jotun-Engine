#include "Mesh.h"

Mesh::Mesh( std::string name ) {
	std::cout << name << " load beginning\n";
	sName = name;
	loadMesh();

	makeGLMesh();
}

Mesh::~Mesh() {
	glDeleteBuffers( 1, &glIndexes );
	glDeleteBuffers( 1, &glVertData );
}

void Mesh::drawShadowPass( GLuint verts ) {
	glEnableVertexAttribArray( verts );
	glBindBuffer( GL_ARRAY_BUFFER, glVertData );
	glVertexAttribPointer( verts, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (0 * sizeof(GLfloat)) );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIndexes );

	glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*) 0 );

	glDisableVertexAttribArray( verts );
}

void Mesh::bind( GLuint bindVerts, GLuint bindUvs, GLuint bindNorms, GLuint bindTangents ) {
	this->verts = bindVerts;
	this->uvs = bindUvs;
	this->norms = bindNorms;
	this->tangents = bindTangents;

	glBindBuffer( GL_ARRAY_BUFFER, glVertData );

	glVertexAttribPointer( verts, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) 0 );
	glVertexAttribPointer( norms, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (3 * sizeof(GL_FLOAT)) );
	glVertexAttribPointer( uvs, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (6 * sizeof(GL_FLOAT)) );
	glVertexAttribPointer( tangents, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (9 * sizeof(GL_FLOAT)) );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIndexes );
}

void Mesh::draw() {
	glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*) 0 );
}

void Mesh::unbind() {
	glDisableVertexAttribArray( verts );
	glDisableVertexAttribArray( uvs );
	glDisableVertexAttribArray( norms );
	glDisableVertexAttribArray( tangents );
}

void Mesh::loadMesh() {
	std::cout << "Loading mesh " << sName << "...\n";
	const aiScene *scene = aiImportFile( sName.c_str(), 
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs );
	const aiMesh *mesh = scene->mMeshes[0];
	for( int i = 0; i < mesh->mNumVertices; i++ ) {
		meshData.push_back( mesh->mVertices[i][0] );
		meshData.push_back( mesh->mVertices[i][1] );
		meshData.push_back( mesh->mVertices[i][2] );

		meshData.push_back( mesh->mNormals[i][0] );
		meshData.push_back( mesh->mNormals[i][1] );
		meshData.push_back( mesh->mNormals[i][2] );

		meshData.push_back( mesh->mTextureCoords[0][i][0] );
		meshData.push_back( mesh->mTextureCoords[0][i][1] );
		meshData.push_back( 0 );

		meshData.push_back( mesh->mTangents[i][0] );
		meshData.push_back( mesh->mTangents[i][1] );
		meshData.push_back( mesh->mTangents[i][2] );
	}

	for( int i = 0; i < mesh->mNumFaces; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			indices.push_back( mesh->mFaces[i].mIndices[j] );
		}
	}
}

void Mesh::makeGLMesh() {
	glGenBuffers( 1, &glVertData );
	glBindBuffer( GL_ARRAY_BUFFER, glVertData );
	glBufferData( GL_ARRAY_BUFFER, meshData.size() * sizeof( GLfloat ), &meshData[0], GL_STATIC_DRAW );

	// Generate a buffer for the indices as well
	glGenBuffers( 1, &glIndexes );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIndexes );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW );
}

std::string Mesh::getName() {
	return sName;
}