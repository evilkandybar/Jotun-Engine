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
out vec3 eyeDirection_tangentspace;
out vec3 lightDirection_tangentspace;
out vec3 halfVector_tangentspace;
out vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPosition_worldspace;
uniform mat4 DepthBiasMVP;

void main() {
    gl_Position =  MVP * vec4( vertexPosition_worldspace, 1 );

    ShadowCoord = DepthBiasMVP * vec4( vertexPosition_worldspace, 1 );

    Position_worldspace = (M * vec4( vertexPosition_worldspace, 1 )).xyz;

    UV = vertexUV.st;

	//normalmapping happens like we transform the light direction into tangent space here once per vertex,
	//rather than transform it in the fragment shader once per fragment

	//get normal, tangent, and bi-tangent
    vec3 n = normalize( (V * M * vec4( vertexNormal_modelspace.xyz, 0 )).xyz ); 
	vec3 t = normalize( (V * M * vec4( vertexTangent_modelspace.xyz, 0 )).xyz );
	vec3 b = cross( n, t );

	vec3 lightDir = normalize( lightPosition_worldspace - vertexPosition_worldspace ).xyz;

	//calculate the light direction in tangent space
	vec3 v;
	v.x = dot( lightDir, t );
	v.y = dot( lightDir, b );
	v.z = dot( lightDir, n );
	lightDirection_tangentspace = normalize( v );

	//calculate the view vector in tangent space
	v.x = dot( vertexPosition_worldspace, t );
	v.y = dot( vertexPosition_worldspace, b );
	v.z = dot( vertexPosition_worldspace, n );
	eyeDirection_tangentspace = normalize( v );

	vec3 vertPosNormalized = normalize( vertexPosition_worldspace );

	//calculate the half vector and send it on (SO MUCH SPECULAR)
	halfVector_tangentspace = normalize( vertPosNormalized + lightDirection_tangentspace );
	halfVector_tangentspace.x = dot( halfVector_tangentspace, t );
	halfVector_tangentspace.y = dot( halfVector_tangentspace, b );
	halfVector_tangentspace.z = dot( halfVector_tangentspace, n );
}