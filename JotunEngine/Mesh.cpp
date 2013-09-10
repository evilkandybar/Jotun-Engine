#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh() : impOut( "ImporterOutput.txt" )
{
	firstRender = true;
}

Mesh::Mesh( std::string name ) : impOut( "ImporterOutput.txt" ) {
	this->name = name;
	import( name );
}

bool Mesh::import( std::string filename ) {
	scene = aiImportFile( filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs );
	return scene;
}

void Mesh::render()
{
	renderNode( scene, scene->mRootNode );
}

//Mostly from http://assimp.svn.sourceforge.net/viewvc/assimp/trunk/samples/SimpleOpenGL/Sample_SimpleOpenGL.c?revision=1332&view=markup
void Mesh::renderNode( const aiScene *sc, const aiNode *nd ) {
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;
	
	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	
	// draw all meshes assigned to this node
	for( ; n < nd->mNumMeshes; ++n ) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
	
		if( mesh->mNormals == NULL ) {
			glDisable( GL_LIGHTING );
		} else {
			glEnable( GL_LIGHTING );
		}
	
		for( t = 0; t < mesh->mNumFaces; ++t ) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;
	
			switch( face->mNumIndices ) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
	
			glBegin( face_mode );
	
			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if( mesh->mTextureCoords[0] != NULL ) {
					glTexCoord2fv( &mesh->mTextureCoords[0][index].x );
				}
				if( mesh->mColors[0] != NULL ) {
					glColor4fv( (GLfloat*)&mesh->mColors[0][index] );
				}
				if( mesh->mNormals != NULL ) {
					glNormal3fv( &mesh->mNormals[index].x );
				}
				glVertex3fv( &mesh->mVertices[index].x );
			}
			glEnd();
		}
	}
	
	// draw all children
	for( n = 0; n < nd->mNumChildren; ++n ) {
		renderNode( sc, nd->mChildren[n] );
	}
	glPopMatrix();
}

Vector3 &Mesh::getVert( int i, Vector3 *out )
{
	out = new Vector3( scene->mMeshes[0]->mVertices[i][0], 
		scene->mMeshes[0]->mVertices[i][1], 
		scene->mMeshes[0]->mVertices[i][2] );
	return *out;
}

Vector3 &Mesh::getNormal( int i, Vector3 *out )
{
	out = new Vector3( scene->mMeshes[0]->mNormals[i][0], 
		scene->mMeshes[0]->mNormals[i][1], 
		scene->mMeshes[0]->mNormals[i][2] );
	return *out;
}

Mesh::~Mesh()
{
	//delete scene;
}