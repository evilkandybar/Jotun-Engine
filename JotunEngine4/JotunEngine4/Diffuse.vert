#version 330 core

in vec3 vertexPosition_modelspace;
uniform mat4 mvp;

void main() {
	vec4 v = vec4( vertexPosition_modelspace, 1 );
	gl_Position = mvp * v;
}