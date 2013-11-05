#version 130

in vec2 UV;
in sampler2D frames[8];

out vec3 fragColor;

void main() {
	fragColor = vec( 0, 0, 0 );
	for( int i = 0; i < 8; i++ ) {
		fragColor += texture( frames[i], i ).rgb;
	}
	fragColor /= 8;
}