#pragma once
#include "stdafx.h"
#include "vboindexer.h"

//loads OBj files and not much else.
//I need to fix that becasue animation
//Or just make racing games...?

/* When you change shader, call mesh.enable()
 * When you change mesh, call mesh.bind()
 * When you actually want to draw the mesh, call mesh.draw()
 * When you are done with the mesh, call mesh.disable()
 */
class Mesh {
public:
	Mesh( std::string name );
	~Mesh();
	void		drawShadowPass( GLuint verts );
	void		enable( GLuint verts, GLuint uvs, GLuint norms );
	void		bind();
	void		draw();
	void		disable(); 
private:
	void		loadMesh();
	void		makeGLMesh();
	std::string sName;
	std::vector<unsigned short> indices;
	std::vector<GLfloat> meshData;
	GLuint glVertData, glIndexes;
	GLuint verts, norms, uvs;
};

