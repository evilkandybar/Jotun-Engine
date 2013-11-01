#version 130

// Input vertex data, different for all executions of this shader.
//it doesn't work, so we'll just get rid of it
in vec3 vertexPosition_worldspace;
in vec3 vertexUV;
in vec3 vertexNormal_modelspace;
in vec3 vertexTangent_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 lightDirection_tangentspace;
out vec3 eyeDirection_tangentspace;
out vec4 ShadowCoord;
out float lightType;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 normalMatrix;
uniform vec4 lightPosition_worldspace;
uniform mat4 DepthBiasMVP;

void main() {
    gl_Position =  MVP * vec4( vertexPosition_worldspace, 1 );

    ShadowCoord = DepthBiasMVP * vec4( vertexPosition_worldspace, 1 );

    Position_worldspace = (M * vec4( vertexPosition_worldspace, 1 )).xyz;

    UV = vertexUV.st;

	//normalmapping happens like we transform the light direction into tangent space here once per vertex,
	//rather than transform it in the fragment shader once per fragment

	//get normal, tangent, and bi-tangent
    vec3 n = normalize( mat3( M ) * vertexNormal_modelspace ); 
	vec3 t = normalize( mat3( M ) * vertexTangent_modelspace );
	vec3 b = cross( n, t );
	
	mat3 worldToTangent;
	worldToTangent[0] = t;
	worldToTangent[1] = b;
	worldToTangent[2] = n;

	worldToTangent = transpose( worldToTangent );

	lightType = lightPosition_worldspace.w;

	vec3 lightDir;
	if( lightPosition_worldspace.w < 0.1 ) {
		lightDir = normalize( lightPosition_worldspace.xyz );
	} else {
		lightDir = normalize( lightPosition_worldspace.xyz - vertexPosition_worldspace );
	}

	//calculate the light direction in tangent space
	lightDirection_tangentspace = normalize( worldToTangent * lightDir );
	//lightDirection_tangentspace = lightDir;

	vec3 vertPosNormalized = normalize( vertexPosition_worldspace );

	//calculate the half vector and send it on (SO MUCH SPECULAR)
	eyeDirection_tangentspace = (M *  gl_Position).xyz;
}