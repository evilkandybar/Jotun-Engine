#include "stdafx.h"
#include "shader.h"

//Shader.h and Shader.cpp taken from http://www.swiftless.com/tutorials/glsl/7_texturing.html

using namespace std;

std::ofstream Shader::debug( "ShaderDebug.txt", std::ios::app );

static char* textFileRead( const char *fileName ) {
	char* text = new char[1];
	if( fileName != NULL ) {
		string filePath( SHADER_PATH );
		filePath += fileName;
        FILE *file;
		fopen_s( &file, fileName, "rt" );
        
		if( file != NULL ) {
            fseek( file, 0, SEEK_END );
            int count = ftell( file );
            rewind( file );
            
			if( count > 0 ) {
				text = (char*)malloc( sizeof(char) * (count + 1) );
				count = fread( text, sizeof(char), count, file );
				text[count] = '\0';
			}
			fclose( file );
		}
		return text;
	}
	return NULL;
}

void Shader::validateShader(GLuint shader, const char* file = 0) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	glGetShaderInfoLog( shader, BUFFER_SIZE, &length, buffer );
	if( length > 0 ) {
		debug << "Shader " << shader << " (" << (file?file:"") << ") compile error: " << buffer << endl;
	}
}

void Shader::validateProgram(GLuint program) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	memset(buffer, 0, BUFFER_SIZE);
	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
	if (length > 0)
		debug << "Program " << program << " link error: " << buffer << endl;
    
	glValidateProgram(program);
	GLint status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
		debug << "Error validating shader " << program << endl;
}

Shader::Shader() {
    
}

Shader::Shader(std::string sname, const char *vsFile, const char *fsFile) {
	int split = sname.find( '.' );
	strName = sname.substr( 0, split );
    init(vsFile, fsFile);
	if( !glIsProgram( id() ) ) {
		debug<<"Program number " <<id() <<", name " <<strName 
			<<" is not a valid shader\n";
	} else {
		debug<<"Program number " <<id() <<", name " <<strName
			<<", is a valid shader\n";
	}
}

void Shader::init(const char *vsFile, const char *fsFile) {
	shader_vp = glCreateShader( GL_VERTEX_SHADER );
	shader_fp = glCreateShader( GL_FRAGMENT_SHADER );
    
	const char* vsText = textFileRead(vsFile);
	const char* fsText = textFileRead(fsFile);	
    
    if (vsText == NULL || fsText == NULL) {
        debug << "Either vertex shader or fragment shader file not found." << endl;
        return;
    }
    
	glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_fp, 1, &fsText, 0);
    
	glCompileShader(shader_vp);
	validateShader(shader_vp, vsFile);
	glCompileShader(shader_fp);
	validateShader(shader_fp, fsFile);
    
	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
	validateProgram(shader_id);
}

Shader::~Shader() {
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);
    
	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

unsigned int Shader::id() {
	return shader_id;
}

void Shader::bind() {
	glUseProgram(shader_id);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::setVariable( std::string variable, Vector3 value ) {
	GLuint var = glGetUniformLocation( shader_id, variable.c_str() );
	glUniform3f( var, value.getX(), value.getY(), value.getZ() );
}

void Shader::setVariable( std::string variable, float value ) {
	GLuint var = glGetUniformLocation( shader_id, variable.c_str() );
	glUniform1f( var, value );
}

void Shader::setVariable( std::string variable, float v1, float v2, float v3, float v4 ) {
	GLuint var = glGetUniformLocation( shader_id, variable.c_str() );
	glUniform4f( var, v1, v2, v3, v4 );
}

void Shader::setVariable( std::string variable, int value ) {
	GLuint var = glGetUniformLocation( shader_id, variable.c_str() );
	glUniform1i( var, value );
}