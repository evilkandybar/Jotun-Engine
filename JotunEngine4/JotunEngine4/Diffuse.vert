#version 330 core

layout( location = 0 ) in vec3 vertex_modelspace;

varying vec3 pos;

void main() {
    gl_Position.xyz = vertex_modelspace;
    gl_Position.w = 1.0;
	pos = gl_Position.xyz;
}