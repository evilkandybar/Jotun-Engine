#include "Mesh.h"


Mesh::Mesh( std::string name ) {
	sName = name;
	std::vector<glm::vec3> normals, vertexes;
	std::vector<glm::vec2> uvs;
	loadMesh( vertexes, uvs, normals );

	indexVBO( vertexes, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals );

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
	glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*) 0           // element array buffer offset
		);
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
<<<<<<< HEAD
	const char *path = sName.c_str();
	printf( "Loading OBJ file %s...\n", path );
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file; fopen_s( &file, path, "r" );
	if( file == NULL ) {
		printf( "Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n" );
		return;
	}
	while( 1 ) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s( file, "%s", lineHeader );
		if( res == EOF )
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if( strcmp( lineHeader, "v" ) == 0 ) {
			glm::vec3 vertex;
			fscanf_s( file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back( vertex );
		} else if( strcmp( lineHeader, "vt" ) == 0 ) {
			glm::vec2 uv;
			fscanf_s( file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back( uv );
		} else if( strcmp( lineHeader, "vn" ) == 0 ) {
			glm::vec3 normal;
			fscanf_s( file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back( normal );
		} else if( strcmp( lineHeader, "f" ) == 0 ) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s( file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if( matches != 9 ) {
				printf( "File can't be read by our simple parser :-( Try exporting with other options\n" );
				return;
			}
			vertexIndices.push_back( vertexIndex[0] );
			vertexIndices.push_back( vertexIndex[1] );
			vertexIndices.push_back( vertexIndex[2] );
			uvIndices.push_back( uvIndex[0] );
			uvIndices.push_back( uvIndex[1] );
			uvIndices.push_back( uvIndex[2] );
			normalIndices.push_back( normalIndex[0] );
			normalIndices.push_back( normalIndex[1] );
			normalIndices.push_back( normalIndex[2] );
		} else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets( stupidBuffer, 1000, file );
=======
		const aiScene *scene = aiImportFile( sName.c_str(), aiProcessPreset_TargetRealtime_Quality );
		const aiMesh *mesh = scene->mMeshes[0];
		for( int i = 0; i < mesh->mNumVertices; i++ ) {
			out_vertices.push_back( glm::vec3( 
				mesh->mVertices[i][0], mesh->mVertices[i][1], mesh->mVertices[i][2] ) );
			out_normals.push_back( glm::vec3(
				mesh->mNormals[i][0], mesh->mNormals[i][1], mesh->mNormals[i][2] ) );
			out_uvs.push_back( glm::vec2( mesh->mTextureCoords[0][i][0], mesh->mTextureCoords[0][i][1] ) );
>>>>>>> cadea8e35c5ce2b1eb362201d956654713972908
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