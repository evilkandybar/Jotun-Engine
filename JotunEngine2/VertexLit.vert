#version 120

uniform mat4 MVP;

void main() {
	gl_Position = MVP * gl_Vertex;
}