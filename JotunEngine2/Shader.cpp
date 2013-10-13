#include "Shader.h"


Shader::Shader( std::string vertName, std::string fragName ) {
	int split = fragName.find_first_of( '.' );
	sName = fragName.substr( 0, split );
	loadShader( vertName.c_str(), fragName.c_str() );
}

Shader::~Shader() {
	glDeleteProgram( glName );
}

void Shader::genAttribMap( std::string *varNames, int numElems ) {
	std::map<std::string, int>::iterator it = attribs.begin();
	int attrib;
	for( int i = 0; i < numElems; i++ ) {
		attrib = glGetAttribLocation( glName, varNames[i].c_str() );
		attribs.insert( it, 
			std::pair<std::string, int>( varNames[i], attrib ) );
	}
}

void Shader::genUniformMap( std::string *varNames, int numElems ) {
	std::map<std::string, int>::iterator it = uniforms.begin();
	int unif;
	for( int i = 0; i < numElems; i++ ) {
		unif = glGetUniformLocation( glName, varNames[i].c_str() );
		uniforms.insert( it,
			std::pair<std::string, int>( varNames[i], unif ) );
	}
}

void Shader::setUniform3f( std::string name, int v1, int v2, int v3 ) {
	glUniform3f( uniforms[name], v1, v2, v3 );
}

void Shader::setUniform1i( std::string name, int v1 ) {
	glUniform1i( uniforms[name], v1 );
}

void Shader::setUniformMat4x4( std::string name, float *value ) {
	glUniformMatrix4fv( uniforms[name], 1, GL_FALSE, value );
}

void Shader::bind() {
	glUseProgram( glName );
}

GLuint Shader::getAttribute( std::string attrib ) {
	return attribs[attrib];
}


//taken from http://www.opengl-tutorial.org/
void Shader::loadShader( const char * vertFile, const char * fragFile ) {

	// Create the shaders
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream( vertFile, std::ios::in );
	if( vertexShaderStream.is_open() ) {
		std::string line = "";
		while( getline( vertexShaderStream, line ) )
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	} else {
		printf( "Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertFile );
		glName = 0;
		return;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream( fragFile, std::ios::in );
	if( fragmentShaderStream.is_open() ) {
		std::string line = "";
		while( getline( fragmentShaderStream, line ) )
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

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

	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );

	glName = programID;
}