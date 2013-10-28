#pragma once
#include "stdafx.h"
/*!\brief Holds data necessary for an OpenGL Shading Language shader*/
class Shader {
public:
	/*!\brief Constructor for a shader, allowing a user to specify the vertex and fragment program
	
	The filenames are relative to the program's working directory

	\param [in] vertName The filename of the vertex program
	\param [in] fragName The filename of the fragment program*/
	Shader( std::string vertName, std::string fragName );
	/*!\brief Default destructor*/
	~Shader();
	/*!\brief Finds the locations for the specified attribute variables and stores them in a std::map
	
	\param [in] varNames The array of attribute names to cache the locations of
	\param [in] numElems The number of attribute locations to cache*/
	void		genAttribMap( std::string varNames [], int numElems );
	/*!\brief Finds the locations for the specified uniform variables and stores them in a std::map

	\param [in] varNames The array of uniform names to cache the locations of
	\param [in] numElems The number of uniform locations to cache*/
	void		genUniformMap( std::string varNames [], int numElems );
	/*!\brief Sends a single integer to the specified location

	Rather than looking up the uniform location every time the data is changed, this function looks
	at a std::map
	
	\param [in] name The name of the uniform variable to give the data to
	\param [in] v1 The integer to send*/
	void		setUniform1i( std::string name, int v1 );
	/*!\brief Sends two integers to the specified location

	Rather than looking up the uniform location every time the data is changed, this function looks
	at a std::map

	\param [in] name The name of the uniform variable to give the data to
	\param [in] v1 The first integer to send
	\param [in] v2 The second integer to send*/
	void		setUniform2i( std::string name, int v1, int v2 );
	/*!\brief Sends three integers to the specified location

	Rather than looking up the uniform location every time the data is changed, this function looks
	at a std::map

	\param [in] name The name of the uniform variable to give the data to
	\param [in] v1 The fisrt integer to send
	\param [in] v2 The second integer to send
	\param [in] v3 The third integer to send*/
	void		setUniform3f( std::string name, int v1, int v2, int v3 );
	/*!\brief Sends a single 4x4 matrix to the specified location

	Rather than looking up the uniform location every time the data is changed, this function looks
	at a std::map

	\param [in] name The name of the uniform variable to give the data to
	\param [in] value The RAM location of the first element of the matrix to send*/
	void		setUniformMat4x4( std::string name, float *value );
	/*!\brief Binds this shader object for use in rendering*/
	void		bind();
	/*!\brief Returns the location of a specified attribute variable
	
	\param [in] attrib The name of the attribute to retrieve the location of
	\return The location of the desired attribute, or -1 if the attribute doesn't exist*/
	GLuint		getAttribute( std::string attrib );
	/*!\brief Returns the integer OpenGL uses to refer to this shader
	
	\return The integer OpenGL uses to refer to this shader*/
	GLuint		getGLName() { return glName; };
	/*!\brief Returns the string name of this shader
	
	The string name is the name of the fragment program minus the file extention. It is not garunteed 
	to be unique*/
	std::string &getSName() { return sName; };
private:
	void		loadShader( const char * vertFile, const char * fragFile );

	std::map<std::string, int> attribs;
	std::map<std::string, int> uniforms;
	std::string sName;
	GLuint		glName;
};