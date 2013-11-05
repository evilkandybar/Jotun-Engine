#version 130

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 eyeDirection_cameraspace;
in mat4 modelMatrix;
in mat4 viewMatrix;
in mat3 tangentToWorld;

out vec4 fragColor;

// Values that stay constant for the whole mesh.
// RGB is the color this thing will emit, A is the scale
uniform sampler2D diffuse;	
uniform float intensity;
uniform sampler2D normalMap;

void main() {
	vec4 color = texture( diffuse, UV );
	vec3 normal = (texture( normalMap, UV ).rgb - vec3( 0.5, 0.5, 0.5 )) * 2;
	normal = (mat3( viewMatrix ) * mat3( modelMatrix ) * tangentToWorld * normal).rgb;
	float lightAmount = dot( normal, normalize( eyeDirection_cameraspace ) );
	fragColor.rgb = mix( vec3( 0.0, 0.0, 0.0 ), color.rgb, lightAmount ) * color.a;// * intensity;
}