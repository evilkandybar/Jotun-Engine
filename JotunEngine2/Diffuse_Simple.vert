#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 DepthBiasMVP;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * gl_Vertex;
	
	ShadowCoord = DepthBiasMVP * gl_Vertex;
	
	// UV of the vertex. No special space for this one.
	UV = gl_TexCoord[0];
}

