#include "Mesh.h"


Mesh::Mesh( std::string mName ) {
	name = mName;
	loadOBJ( mName );
	makeGLMesh();
}


Mesh::~Mesh() {
	glDeleteVertexArrays( 1, &vertexArrayID );
}

void Mesh::draw() {
	glEnableVertexAttribArray( 0 );
	bind();
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	//glBegin(GL_TRIANGLES);
	//glColor3f(1, 0, 0);
	//glVertex3f(1, 0, 0);
	//glColor3f(0, 1, 0);
	//glVertex3f(-1, 0, 0);
	//glColor3f(0, 0, 1);
	//glVertex3f(0, 1, 0);
	//glEnd();
}

void Mesh::loadOBJ( std::string fileToLoad ) {
	std::ifstream file( fileToLoad.c_str() );
	std::vector<float> vVertexes;
	std::vector<float> vnormals;
	std::vector<float> vtexCoords;

	std::vector<int> vVertexOrder;
	std::vector<int> vnormalOrder;
	std::vector<int> vtexCoordOrder;
	if( file.is_open() ) {
		std::string line;
		char *buf = new char[128];
		int start, end;
		while( !file.eof() ) {
			file.getline( buf, 128 );
			line = buf;

			if( line.find( "vn" ) != std::string::npos ) { //if we're looking at a line with a normal 
				start = 3;	//the first position of a numberic value
				end = line.find( ' ', start + 1 );
				vnormals.push_back( atof( (line.substr( start, end )).c_str() ) );

				start = line.find( ' ', end + 1 );
				end = line.find( ' ', start + 1 );
				vnormals.push_back( atof( (line.substr( start, end )).c_str() ) );

				start = line.find( ' ', end + 1 );
				end = line.size();
				vnormals.push_back( atof( (line.substr( start, end )).c_str() ) );
			} else if( line.find( "vt" ) != std::string::npos ) { //if we're looking at a line with a texture coordinate
				start = 3;	//the first position of a numberic value
				end = line.find( ' ', start + 1 );
				vtexCoords.push_back( atof( (line.substr( start, end )).c_str() ) );

				start = line.find( ' ', end + 1 );
				end = line.size();
				vtexCoords.push_back( atof( (line.substr( start, end )).c_str() ) );
			} else if( line.find( "v" ) != std::string::npos ) { //if we're looking at a line with a vertex 
				start = 2;	//the first position of a numberic value
				end = line.find( ' ', start + 1 );
				vVertexes.push_back( atof( (line.substr( start, end )).c_str() ) );

				start = line.find( ' ', end + 1 );
				end = line.find( ' ', start + 1 );
				vVertexes.push_back( atof( (line.substr( start, end )).c_str() ) );

				start = line.find( ' ', end + 1 );
				end = line.size();
				vVertexes.push_back( atof( (line.substr( start, end )).c_str() ) );
			} else if( line.find( "f" ) != std::string::npos ) { //if we're looking at a line with a face
				//There's a pretty good chance that all files will have vretexes, normals, and texture coordinates,
				//so we're goig to yell if they don't
				//Notice that this place is reached after all the vertexes and texture coordinates and whatnot
				if( vnormals.size() == 0 || vtexCoords.size() == 0 ) {
					std::cerr << "ERROR: Data file " << fileToLoad << " does not contain normals or UV coordinates. Aborting...";
					return;
				}
				start = 2;
				end = line.find( '/' );
				vVertexOrder.push_back( atoi( line.substr( start, end ).c_str() ) );

				start = end + 1;;
				end = line.find( '/', start );
				vtexCoords.push_back( atoi( line.substr( start, end ).c_str() ) );

				start = end + 1;;
				end = line.size();
				vnormalOrder.push_back( atoi( line.substr( start, end ).c_str() ) );
			}
		}
		//vVertexOrder is where the normals and texture coords need to be
		//vnormalOrder is where the normals are now
		//so just swap vnormals[vnormalOrder[i]] with vnormals[vVertexOrder[i]]
		float temp;
		for( int i = 0; i < vVertexOrder.size(); i++ ) {
			//swap normals
			temp = vnormals[vVertexOrder[i] * 3];
			vnormals[vVertexOrder[i] * 3] = vnormals[vnormalOrder[i] * 3];
			vnormals[vnormalOrder[i] * 3] = temp;
			temp = vnormals[vVertexOrder[i] * 3 + 1];
			vnormals[vVertexOrder[i] * 3 + 1] = vnormals[vnormalOrder[i] * 3 + 1];
			vnormals[vnormalOrder[i] * 3 + 1] = temp;
			temp = vnormals[vVertexOrder[i] * 3 + 2];
			vnormals[vVertexOrder[i] * 3 + 2] = vnormals[vnormalOrder[i] * 3 + 2];
			vnormals[vnormalOrder[i] * 3 + 2] = temp;

			//swap uv coords
			temp = vtexCoords[vVertexOrder[i] * 2];
			vtexCoords[vVertexOrder[i] * 2] = vtexCoords[vtexCoordOrder[i] * 2];
			vtexCoords[vtexCoordOrder[i] * 2] = temp;
			temp = vnormals[vVertexOrder[i] * 2 + 1];
			vtexCoords[vVertexOrder[i] * 2 + 1] = vtexCoords[vtexCoordOrder[i] * 2 + 1];
			vtexCoords[vtexCoordOrder[i] * 2 + 1] = temp;
		}
	}
}

void Mesh::makeGLMesh() {
	glGenVertexArrays( 1, &vertexArrayID );
	glBindVertexArray( vertexArrayID );

	glGenBuffers( 1, &vertexBufferID );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
	glBufferData( GL_ARRAY_BUFFER, sizeof(numVerts * 3 * sizeof(float) ), vertexes, GL_STATIC_DRAW );
}

void Mesh::bind() {
	glBindVertexArray( vertexArrayID );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
}