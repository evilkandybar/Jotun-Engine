#version 130

// Input vertex data, different for all executions of this shader.
//it doesn't work, so we'll just get rid of it
attribute vec3 vertexPosition_modelspace;
attribute vec3 vertexUV;
attribute vec3 vertexNormal_modelspace;
attribute vec3 vertexTangent_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightInvDirection_worldspace;
uniform mat4 DepthBiasMVP;
uniform sampler2D normalMap;

attribute vec3 vTangent;

void main() {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4( vertexPosition_modelspace, 1 );
	
	ShadowCoord = DepthBiasMVP *  vec4( vertexPosition_modelspace, 0 );
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = ( M * vec4( vertexPosition_modelspace, 0 ) ).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	EyeDirection_cameraspace = vec3( 0, 0, 0 ) - ( V * M *  vec4( vertexPosition_modelspace, 0 ) ).xyz;

	// Vector that goes from the vertex to the light, in camera space
	LightDirection_cameraspace = ( V * vec4( LightInvDirection_worldspace, 0 ) ).xyz;
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV.st;
	
	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	Normal_cameraspace = ( V * M * vec4( vertexNormal_modelspace.xyz, 0 ) ).xyz; 
}