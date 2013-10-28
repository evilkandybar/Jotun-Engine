#pragma once
#include "stdafx.h"
#include "vboindexer.h"

/*!\brief Defines a 3D mesh.

This class can load and render and static mesh. Animation support coming soon

 * When you change mesh, call mesh.bind()
 * When you actually want to draw the mesh, call mesh.draw()
 * When you are done with the mesh, call mesh.disable()

 Note that this isn't necessary until I have VBOs working

 For now, just call draw()
 */
class Mesh {
public:
	/*!\brief Loads a mesh from the spceified file name

	As this class uses Assimp, it is constrained by that library's capabilities.
	A list of supported meshes can be found at 
	http://assimp.sourceforge.net/lib_html/index.html
	
	\param [in] name The filename of the mesh to load*/
	Mesh( std::string name );
	/*!\brief Default destructor*/
	~Mesh();
	/*!\brief Draws the mesh, specifying only vertexes
	
	\param [in] verts The shader's location for vertex position data*/	
	void		drawShadowPass( GLuint verts );
	/*!\brief Tells the shader to use this mesh's data in rendering
	
	\param [in] bindVerts The location in the shader of the vertex position data
	\param [in] bindUvs The location in the shader of the vertex UV data
	\param [in] bindNorms The location in the shader of the vertex normal data
	\param [in] bindTangents The location in the shader of the vertex tangent
		data*/
	void		bind( GLuint bindVerts, 
					  GLuint bindUvs, 
					  GLuint bindNorms, 
					  GLuint bindTangents );
	/*!\brief Renders the mesh data
	
	Currently, this function uses glVertex3fv, glNormal3fv, and glTexCoord3fv to specify mesh data
	Rendering with glDrawElements will (hopefully) happen soon*/
	void		draw();
	/*!\brief Tells the current shader to no longer use our data
	
	This is important when switching shaders or switching meshes (probably?)*/
	void		unbind(); 
private:
	void		loadMesh();
	void		makeGLMesh();
	std::string sName;
	std::vector<unsigned short> indices;
	std::vector<GLfloat> meshData;
	GLuint		glVertData, glIndexes;
	GLuint		verts, norms, uvs, tangents;
};