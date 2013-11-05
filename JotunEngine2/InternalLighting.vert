#version 130

in vec3 vertexPosition_worldspace;
in vec3 vertexUV;
in vec3 vertexNormal_modelspace;
in vec3 vertexTangent_modelspace;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 eyeDirection_cameraspace;
out mat4 modelMatrix;
out mat4 viewMatrix;
out mat3 tangentToWorld;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

void main() {
	modelMatrix = M;
	viewMatrix = V;
    gl_Position =  MVP * vec4( vertexPosition_worldspace, 1 );

    Position_worldspace = (M * vec4( vertexPosition_worldspace, 1 )).xyz;

    UV = vertexUV.st;

	//get normal, tangent, and bi-tangent
    vec3 n = normalize( mat3( M ) * vertexNormal_modelspace ); 
	vec3 t = normalize( mat3( M ) * vertexTangent_modelspace );
	vec3 b = cross( n, t );
	
	mat3 worldToTangent;
	worldToTangent[0] = t;
	worldToTangent[1] = b;
	worldToTangent[2] = n;

	tangentToWorld = worldToTangent;

	eyeDirection_cameraspace = vec3( 0, 0, 0 ) - (V * M * vec4( vertexPosition_worldspace, 1 )).xyz;
}