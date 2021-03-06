#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform sampler2DShadow shadowMap;

void main(){

	// Light emission properties
	vec3 LightColor = vec3(1,1,1);
	
	// Material properties
	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;

	float visibility = shadow2D( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) ).r;

	gl_FragColor.rgb = visibility * MaterialDiffuseColor * LightColor;

}