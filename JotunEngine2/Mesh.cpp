#include "Mesh.h"

Mesh::Mesh( std::string name ) {
	sName = name;
	loadMesh();

	makeGLMesh();
}

Mesh::~Mesh() {
	glDeleteBuffers( 1, &glIndexes );
	glDeleteBuffers( 1, &glVertData );
}

void Mesh::drawShadowPass( GLuint verts ) {
	// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray( verts );
	//glBindBuffer( GL_ARRAY_BUFFER, glVertData );
	//glVertexAttribPointer( verts, 3, GL_FLOAT, GL_FALSE, 8, (void*) 0 );

	// Index buffer
	//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIndexes );

	// Draw the triangles !
	//glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*) 0 );
	glBegin( GL_TRIANGLES );
	for( int i = 0; i < indices.size(); i++ ) {
		glVertex3fv( &meshData[indices[i] * 12] );
	}
	glEnd();

	//glDisableVertexAttribArray( verts );
}

void Mesh::bind( GLuint bindVerts, GLuint bindUvs, GLuint bindNorms, GLuint bindTangents ) {
	this->verts = bindVerts;
	this->uvs = bindUvs;
	this->norms = bindNorms;
	this->tangents = bindTangents;

	// 1rst attribute buffer : vertices
	glBindBuffer( GL_ARRAY_BUFFER, glVertData );
	glVertexAttribPointer( verts, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0 );

	// 2nd attribute buffer : normals
	glVertexAttribPointer( norms, 3, GL_FLOAT, GL_FALSE, 12, (void*) 3 ); 

	//3rd attribute buffer : UVs
	glVertexAttribPointer( uvs, 3, GL_FLOAT, GL_FALSE, 12, (void*) 6 );

	//4th attribute buffer: tangents
	glVertexAttribPointer( tangents, 3, GL_FLOAT, GL_FALSE, 12, (void*) 9 );

	// Index buffer
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIndexes );
}

void Mesh::draw() {
	//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIndexes );
	//glDrawElements( GL_LINES, indices.size(), GL_UNSIGNED_SHORT, (void*) 0 );
	glBegin( GL_TRIANGLES );
	int ind;
	for( int i = 0; i < indices.size(); i++ ) {
		ind = indices[i] * 12;
		glNormal3fv( &meshData[ind + 3] );
		glTexCoord2fv( &meshData[ind + 6] );
		glVertex3fv( &meshData[ind] );
	}
	glEnd();
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