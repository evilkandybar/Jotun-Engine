#include "Mesh.h"


Mesh::Mesh( std::string name ) {
	sName = name;
	std::vector<glm::vec3> normals, vertexes;
	std::vector<glm::vec2> uvs;
	loadMesh( indexed_vertices, indexed_uvs, indexed_normals );
	
	for( int i = 0; i < indexed_vertices.size(); i++ ) {
		indices.push_back( i );
	}

	//indexVBO( vertexes, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals );

	makeGLMesh();
}

Mesh::~Mesh() {
	glDeleteBuffers( 1, &vertexbuffer );
	glDeleteBuffers( 1, &uvbuffer );
	glDeleteBuffers( 1, &normalbuffer );
	glDeleteBuffers( 1, &elementbuffer );
}

void Mesh::drawShadowPass( GLuint verts ) {
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray( verts );
	glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
	glVertexAttribPointer(
		verts,  // The attribute we want to configure
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
		);

	// Index buffer
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementbuffer );

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*) 0           // element array buffer offset
		);

	glDisableVertexAttribArray( verts );
}

void Mesh::enable( GLuint bindVerts, GLuint bindUvs, GLuint bindNorms ) {
	this->verts = bindVerts;
	this->uvs = bindUvs;
	this->norms = bindNorms;
	glEnableVertexAttribArray( verts );
	glEnableVertexAttribArray( uvs );
	glEnableVertexAttribArray( norms );
}

void Mesh::bind() {
	// 1rst attribute buffer : vertices
	glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
	glVertexAttribPointer(
		verts,  // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*) 0                      // array buffer offset
		);

	// 2nd attribute buffer : UVs
	glBindBuffer( GL_ARRAY_BUFFER, uvbuffer );
	glVertexAttribPointer(
		uvs,                   // The attribute we want to configure
		2,                            // size : U+V => 2
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*) 0                      // array buffer offset
		);

	// 3rd attribute buffer : normals
	glBindBuffer( GL_ARRAY_BUFFER, normalbuffer );
	glVertexAttribPointer(
		norms,    // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*) 0                      // array buffer offset
		);

	// Index buffer
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementbuffer );
}

void Mesh::draw() {
	// Draw the triangles !
	/*glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*) 0           // element array buffer offset
			);*/
	glBegin( GL_TRIANGLES );
	for( int i = 0; i < indexed_vertices.size(); i++ ) {
		glVertex3fv( &indexed_vertices[i][0] );
		glNormal3fv( &indexed_normals[i][0] );
		glTexCoord2fv( &indexed_uvs[i][0] );
	}
	glEnd();
}

void Mesh::disable() {
	glDisableVertexAttribArray( verts );
	glDisableVertexAttribArray( uvs );
	glDisableVertexAttribArray( norms );
}

void Mesh::loadMesh(
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals 
	) {
		const aiScene *scene = aiImportFile( sName.c_str(), aiProcessPreset_TargetRealtime_Quality );
		const aiMesh *mesh = scene->mMeshes[0];
		for( int i = 0; i < mesh->mNumVertices; i++ ) {
			out_vertices.push_back( glm::vec3( 
				mesh->mVertices[i][0], mesh->mVertices[i][1], mesh->mVertices[i][2] ) );
			out_normals.push_back( glm::vec3(
				mesh->mNormals[i][0], mesh->mNormals[i][1], mesh->mNormals[i][2] ) );
			out_uvs.push_back( glm::vec2( mesh->mTextureCoords[0][i][0], mesh->mTextureCoords[0][i][1] ) );
		}
}

void Mesh::makeGLMesh() {
	glGenBuffers( 1, &vertexbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
	glBufferData( GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &uvbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, uvbuffer );
	glBufferData( GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &normalbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, normalbuffer );
	glBufferData( GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW );

	// Generate a buffer for the indices as well
	glGenBuffers( 1, &elementbuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementbuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW );
}