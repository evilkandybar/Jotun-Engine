#version 130

// Input vertex data, different for all executions of this shader.
//it doesn't work, so we'll just get rid of it
in vec3 vertexPosition_modelspace;
in vec3 vertexUV;
in vec3 vertexNormal_modelspace;
in vec3 vertexTangent_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
//The W tells us which sort of light this it. 0 = point, 1 = directionsl, 2 = spot
//If the light is a directional light, the xyz tells us its direction. Otherwise, 
out vec4 LightDirections_cameraspace[8];
out vec4 LightColors[8];
out vec4 ShadowCoord;
out int numLights;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightInvDirection_worldspace;
uniform mat4 DepthBiasMVP;
//The W tells us which sort of light this it. 0 = point, 1 = directionsl, 2 = spot
uniform vec4 lightPositions_worldspace[8];
uniform vec4 lightColors[8];
uniform int inNumLights;

void main() {
    gl_Position =  MVP * vec4( vertexPosition_modelspace, 1 );

    ShadowCoord = DepthBiasMVP * vec4( vertexPosition_modelspace, 1 );

    Position_worldspace = (M * vec4( vertexPosition_modelspace, 1 )).xyz;

    EyeDirection_cameraspace = vec3( 0, 0, 0 ) - (V * M * vec4( vertexPosition_modelspace, 1 )).xyz;

	for( int i = 0; i < numLights; i++ ) {
		if( lightPositions_worldspace[i].w < 0.1 ) {
			//Treat the light as a point light Find the distance, do not normalize it
			LightDirections_cameraspace[i].xyz = (V * vec4( lightPositions_worldspace[i].xyz - gl_Position.xyz, 1 )).xyz;
			LightDirections_cameraspace[i].w = lightPositions_worldspace[i].w;
		} else if( lightPositions_worldspace[i]	.w < 1.1 ) {
			//treat the light as a directional light
			LightDirections_cameraspace[i] = vec4( (M * vec4(lightPositions_worldspace[i].xyz, 1 )).xyz, 
													lightPositions_worldspace[i].w );
		}
		//spot lights can happen later
	}

    UV = vertexUV.st;

    Normal_cameraspace = (V * M * vec4( vertexNormal_modelspace.xyz, 0 )).xyz; 
}