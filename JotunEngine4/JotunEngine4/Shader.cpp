#include "Shader.h"

Shader::Shader(const char *sName) {
	name = sName;
	loadShader(name);
}

Shader::Shader(std::string &sName) {
	name = sName;
	loadShader(name);
}

Shader::~Shader() {
}

void Shader::bind() {
	glUseProgram(glid);
	if (!glIsProgram(glid)) {
		int j;
		j = 5;
	}
}

int Shader::getGLID() {
	return glid;
}

int Shader::loadShader(std::string &name) {
	std::string vertName = name + ".vert";
	std::string fragName = name + ".frag";

	//check as early a possible to see if there's any problems to save CPU cycles
	std::ifstream vertFile(vertName.c_str());
	if (!vertFile.is_open()) {
		std::cerr << "ERROR: Could not open vertex shader " << vertName << "\n";
		return -1;
	}

	std::ifstream fragFile(fragName.c_str());
	if (!fragFile.is_open()) {
		std::cerr << "ERROR: Could not open fragment shader " << fragName << "\n";
		return -1;
	}

	std::stringstream buffer;
	buffer << vertFile.rdbuf();
	std::string vertShader = buffer.str();

	buffer.str(std::string());
	buffer << fragFile.rdbuf();
	std::string fragShader = buffer.str();

	GLuint vertGLID, fragGLID;

	vertGLID = glCreateShader(GL_VERTEX_SHADER);
	fragGLID = glCreateShader(GL_FRAGMENT_SHADER);

	if (vertGLID == 0 || fragGLID == 0) {
		std::cerr << "ERROR: Could not create a shader\n";
		std::cin.get();
		return -1;
	}

	const char *charVertBuf = vertShader.c_str();
	glShaderSource(vertGLID, 1, &charVertBuf, NULL);

	const char *charFragBuf = fragShader.c_str();
	glShaderSource(fragGLID, 1, &charFragBuf, NULL);

	glCompileShader(vertGLID);
	validateShader(vertGLID, charVertBuf);
	glCompileShader(fragGLID);
	validateShader(fragGLID, charFragBuf);

	glid = glCreateProgram();

	glAttachShader(glid, fragGLID);
	glAttachShader(glid, vertGLID);
	glLinkProgram(glid);

	return 0;
}

void Shader::validateShader(GLuint shader, const char* file) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;

	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	if (length > 0) {
		std::cerr << "Shader " << shader << " (" << (file ? file : "") << ") compile error: " << buffer << "\n";
	}
}