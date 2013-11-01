#include "Shader.h"

Shader::Shader( std::string vertName, std::string fragName ) {
	int split = fragName.find_first_of( '.' );
	sName = fragName.substr( 0, split );
	loadShader( vertName.c_str(), fragName.c_str() );
}

Shader::~Shader() {
	glDeleteProgram( glName );
}

void Shader::genAttribMap() {
	std::map<std::string, int>::iterator it = attribs.begin();
	int attrib;
	for( int i = 0; i < attribNames.size(); i++ ) {
		attrib = glGetAttribLocation( glName, attribNames[i].c_str() );
		attribs.insert( it, std::pair<std::string, int>( attribNames[i], attrib ) );
	}
}

void Shader::genAttribMap( std::string *varNames, int numElems ) {
	std::map<std::string, int>::iterator it = attribs.begin();
	int attrib;
	for( int i = 0; i < numElems; i++ ) {
		attrib = glGetAttribLocation( glName, varNames[i].c_str() );
		attribs.insert( it, std::pair<std::string, int>( varNames[i], attrib ) );
	}
}

void Shader::genUniformMap() {
	std::map<std::string, int>::iterator it = uniforms.begin();
	int unif;
	for( int i = 0; i < uniformNames.size(); i++ ) {
		unif = glGetUniformLocation( glName, uniformNames[i].c_str() );
		uniforms.insert( it, std::pair<std::string, int>( uniformNames[i], unif ) );
	}
}

void Shader::genUniformMap( std::string *varNames, int numElems ) {
	std::map<std::string, int>::iterator it = uniforms.begin();
	int unif;
	for( int i = 0; i < numElems; i++ ) {
		unif = glGetUniformLocation( glName, varNames[i].c_str() );
		uniforms.insert( it, std::pair<std::string, int>( varNames[i], unif ) );
	}
}

void Shader::setUniform3f( std::string name, float v1, float v2, float v3 ) {
	glUniform3f( uniforms[name], v1, v2, v3 );
}

void Shader::setUniform4f( std::string name, float v1, float v2, float v3, float v4 ) {
	glUniform4f( uniforms[name], v1, v2, v3, v4 );
}

void Shader::setUniform1i( std::string name, int v1 ) {
	glUniform1i( uniforms[name], v1 );
}

void Shader::setUniform2i( std::string name, int v1, int v2 ) {
	glUniform2i( uniforms[name], v1, v2 );
}

void Shader::setUniformf4Array( std::string name, int num, float4 *vs ) {
	glUniform4fv( uniforms[name], num, &vs->x );
}

void Shader::setUniformMat3x3( std::string name, float *value ) {
	glUniformMatrix3fv( uniforms[name], 1, GL_FALSE, value );
}

void Shader::setUniformMat4x4( std::string name, float *value ) {
	glUniformMatrix4fv( uniforms[name], 1, GL_FALSE, value );
}

void Shader::bind() {
	glUseProgram( glName );
	for( std::pair<const std::string, int> &p : attribs ) {
		glEnableVertexAttribArray( p.second );
	}
}

GLuint Shader::getAttribute( std::string attrib ) {
	return attribs[attrib];
}

//taken from http://www.opengl-tutorial.org/
void Shader::loadShader( const char * vertFile, const char * fragFile ) {

	// Create the shaders
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	int pos;
	int endPos;

#pragma region LoadVertexShader
	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream( vertFile, std::ios::in );
	if( vertexShaderStream.is_open() ) {
		std::string line = "";
		//parse each line, looking for "uniform" to generate a uniform map
		//also look for "in" to generate the attribute map
		while( getline( vertexShaderStream, line ) ) {
			pos = line.find( "uniform" );
			if( pos == 0 ) {
				pos = line.find( ' ', pos + 9 ) + 1;
				endPos = line.find( '[' ) - 1;
				if( endPos == std::string::npos - 1 ) {
					endPos = line.find( ';' );
				}
				uniformNames.push_back( line.substr( pos, endPos - pos ) );
			}
			pos = line.find( "in" );
			if( pos == 0 ) {
				pos = line.find( ' ', pos + 3 ) + 1;
				endPos = line.find( ';' );
				attribNames.push_back( line.substr( pos, endPos - pos ) );
			}
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	} else {
		printf( "Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertFile );
		glName = 0;
		return;
	}
#pragma endregion
#pragma region LoadFragmentShader
	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream( fragFile, std::ios::in );
	if( fragmentShaderStream.is_open() ) {
		std::string line = "";
		//parse each line, looking for "uniform" to generate a uniform map
		while( getline( fragmentShaderStream, line ) ) {
			pos = line.find( "uniform" );
			if( pos == 0 ) {
				pos = line.find( ' ', pos + 9 ) + 1;
				endPos = line.find( '[' ) - 1;
				if( endPos == std::string::npos - 1 ) {
					endPos = line.find( ';' );
				}
				uniformNames.push_back( line.substr( pos, endPos - pos ) );
			}
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	}
#pragma endregion

	GLint result = GL_FALSE;
	int infoLogLength;

#pragma region CompileVertexShader
	// Compile Vertex Shader
	printf( "Compiling shader : %s\n", vertFile );
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource( vertexShaderID, 1, &vertexSourcePointer, NULL );
	glCompileShader( vertexShaderID );

	// Check Vertex Shader
	glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
	if( infoLogLength > 0 ) {
		std::vector<char> vertexShaderErrorMessage( infoLogLength + 1 );
		glGetShaderInfoLog( vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0] );
		printf( "%s\n", &vertexShaderErrorMessage[0] );
	}
#pragma endregion
#pragma region CompileFragmentShader
	// Compile Fragment Shader
	printf( "Compiling shader : %s\n", fragFile );
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource( fragmentShaderID, 1, &fragmentSourcePointer, NULL );
	glCompileShader( fragmentShaderID );

	// Check Fragment Shader
	glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
	if( infoLogLength > 0 ) {
		std::vector<char> fragmentShaderErrorMessage( infoLogLength + 1 );
		glGetShaderInfoLog( fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0] );
		printf( "%s\n", &fragmentShaderErrorMessage[0] );
	}
#pragma endregion
#pragma region LinkProgram
	// Link the program
	printf( "Linking program\n" );
	GLuint programID = glCreateProgram();
	glAttachShader( programID, vertexShaderID );
	glAttachShader( programID, fragmentShaderID );
	glLinkProgram( programID );

	// Check the program
	glGetProgramiv( programID, GL_LINK_STATUS, &result );
	glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength );
	if( infoLogLength > 0 ) {
		std::vector<char> programErrorMessage( infoLogLength + 1 );
		glGetProgramInfoLog( programID, infoLogLength, NULL, &programErrorMessage[0] );
		printf( "%s\n", &programErrorMessage[0] );
	}
#pragma endregion

	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );

	glName = programID;
	genAttribMap();
	genUniformMap();
}