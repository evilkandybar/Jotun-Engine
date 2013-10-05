#version 330 core

out vec3 color;

varying vec3 pos;

void main() {
	color = pos;
}