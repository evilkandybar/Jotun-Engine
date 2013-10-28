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
out vec3 LightDirection_cameraspace;
out vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightInvDirection_worldspace;
uniform mat4 DepthBiasMVP;

void main() {
    gl_Position =  MVP * vec4( vertexPosition_modelspace, 1 );

    ShadowCoord = DepthBiasMVP * vec4( vertexPosition_modelspace, 1 );

    Position_worldspace = (M * vec4( vertexPosition_modelspace, 1 )).xyz;

    EyeDirection_cameraspace = vec3( 0, 0, 0 ) - (V * M * vec4( vertexPosition_modelspace, 1 )).xyz;

    LightDirection_cameraspace = (V * vec4( LightInvDirection_worldspace, 0 )).xyz;

    UV = vertexUV.st;

    Normal_cameraspace = (V * M * vec4( vertexNormal_modelspace.xyz, 0 )).xyz; 
}