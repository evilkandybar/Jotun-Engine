#include "ShaderList.h"

//Taken from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
Shader* ShaderList::loadShader( std::string name ) {
	Shader *ret = new Shader( name );
	std::string fragName = name + ".frag";
	std::string vertName = name + ".vert";
    // Create the shaders
    GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
    GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
 
    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream( vertName.c_str(), std::ios::in );
    if( vertexShaderStream.is_open() )
    {
        std::string line = "";
        while( getline( vertexShaderStream, line ) ) {
            vertexShaderCode += "\n" + line;
		}
        vertexShaderStream.close();
    }
 
    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream( fragName, std::ios::in );
    if( fragmentShaderStream.is_open() ) {
        std::string line = "";
        while( getline( fragmentShaderStream, line ) ) {
            fragmentShaderCode += "\n" + line;
		}
        fragmentShaderStream.close();
    }
 
    GLint result = GL_FALSE;
    int infoLogLength;
 
    // Compile Vertex Shader
    printf( "Compiling shader : %s\n", vertName );
    char const * vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource( vertexShaderID, 1, &vertexSourcePointer , NULL );
    glCompileShader( vertexShaderID );
 
    // Check Vertex Shader
    glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, &result );
    glGetShaderiv( vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
    std::vector<char> vertexShaderErrorMessage( infoLogLength );
    glGetShaderInfoLog( vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0] );
    fprintf( stdout, "%s\n", &vertexShaderErrorMessage[0] );
 
    // Compile Fragment Shader
    printf( "Compiling shader : %s\n", fragName );
    char const * fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource( fragmentShaderID, 1, &fragmentSourcePointer, NULL );
    glCompileShader( fragmentShaderID );
 
    // Check Fragment Shader
    glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, &result );
    glGetShaderiv( fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
    std::vector<char> fragmentShaderErrorMessage( infoLogLength );
    glGetShaderInfoLog( fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0] );
    fprintf( stdout, "%s\n", &fragmentShaderErrorMessage[0] );
 
    // Link the program
    fprintf( stdout, "Linking program\n" );
    GLuint programID = glCreateProgram();
    glAttachShader( programID, vertexShaderID );
    glAttachShader( programID, fragmentShaderID );
    glLinkProgram( programID );
 
    // Check the program
    glGetProgramiv( programID, GL_LINK_STATUS, &result );
    glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength );
    std::vector<char> programErrorMessage( std::max( infoLogLength, int(1) ) );
    glGetProgramInfoLog( programID, infoLogLength, NULL, &programErrorMessage[0] );
    fprintf( stdout, "%s\n", &programErrorMessage[0] );
	
	glDeleteShader( vertexShaderID );
    glDeleteShader( fragmentShaderID );
 
	ret->setID( programID );

    return ret;
}