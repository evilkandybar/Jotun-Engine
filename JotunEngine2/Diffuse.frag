#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform sampler2D diffuse;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform sampler2D shadowMap;
//uniform int shadowLevel;	//0 is no shadow, 1 is hard shadows, 2 is soft shadows, 3 is PCSS

vec2 poissonDisk( int ind ) {
   if( ind == 0 ) {
		return vec2( -0.1094937f, -0.752005 );
   } 
   if( ind == 1 ) {
		return vec2( 0.5059697f, -0.7294227f );
   } 
   if( ind == 2 ) {
		return vec2( -0.3904303f, 0.5678311f );
   }  
   if( ind == 3 ) {
		return vec2( -0.3050305f, 0.7459931f );
   }  
   if( ind == 4 ) {
		return vec2( 0.1725386f, -0.506364f );
   }  
   if( ind == 5 ) {
		return vec2( 0.1979104f, 0.7830779f );
   }  
   if( ind == 6 ) {
		return vec2( 0.0663829f, 0.9336991f );
   }  
   if( ind == 7 ) {
		return vec2( -0.163072f, -0.9741971f );
   }  
   if( ind == 8 ) {
		return vec2( 0.1710306f, 0.5527771f );
   }  
   if( ind == 9 ) {
		return vec2( 0.02903906f, 0.3999698f );
   }  
   if( ind == 10 ) {
		return vec2( -0.1748933f, 0.1948632f );
   }  
   if( ind == 11 ) {
		return vec2( -0.3564819f, 0.2770886f );
   }  
   if( ind == 12 ) {
		return vec2( -0.4994766f, -0.4100508f );
   }  
   if( ind == 13 ) {
		return vec2( 0.6305282f, -0.5586912f );
   }  
   if( ind == 14 ) {
		return vec2( -0.5874177f, -0.1295959f );
   }  
   if( ind == 15 ) {
		return vec2( 0.4260757f, -0.02231212f );
   }  
   if( ind == 16 ) {
		return vec2( -0.8381009f, -0.1279669f );
   }  
   if( ind == 17 ) {
		return vec2( -0.8977778f, 0.1717084f );
   }  
   if( ind == 18 ) {
		return vec2( 0.8211543f, 0.365194f );
   }  
   if( ind == 19 ) {
		return vec2( 0.6365152f, -0.229197f );
   }  
   if( ind == 20 ) {
		return vec2( -0.8206947f, -0.3301564f );
   }  
   if( ind == 21 ) {
		return vec2( 0.08938109f, -0.005763604f );
   }  
   if( ind == 22 ) {
		return vec2( -0.3123821f, 0.2344262f );
   }  
   if( ind == 23 ) {
		return vec2( 0.1038207f, -0.2167438f );
   }  
   if( ind == 24 ) {
		return vec2( 0.3256707f, 0.2347208f );
   }  
   if( ind == 25 ) {
		return vec2( 0.3405131f, 0.4458854f );
   }  
   if( ind == 26 ) {
		return vec2( -0.6740047f, -0.4649915f );
   }  
   if( ind == 27 ) {
		return vec2( -0.6670403f, 0.658087f );
   }  
   if( ind == 28 ) {
		return vec2( -0.4680224f, -0.4418066f );
   }  
   if( ind == 29 ) {
		return vec2( 0.09780561f, -0.1236207f );
   }  
   if( ind == 30 ) {
		return vec2( -0.030519f, 0.3487186f );
   }  
   if( ind == 31 ) {
		return vec2( 0.4240496f, -0.1010172f );
   }  
}

// Returns a random number based on a vec3 and an int.
float random( vec3 seed, int i ) {
	vec4 seed4 = vec4( seed, i );			
	float dot_product = dot( seed4, vec4( 12.9898, 78.233, 45.164, 94.673 ) );
	return fract( sin( dot_product ) * 43758.5453 );
}

int mod( int a, int b ) {
	return a - (a / b);
}

void main() {
int shadowLevel = 1;	//let's just do hard shadows
	// Light emission properties
	vec3 LightColor = vec3( 1, 1, 1 );
	float LightPower = 1.0f;

	// Material properties
	vec3 MaterialDiffuseColor = texture2D( diffuse, UV ).rgb;
	vec3 MaterialAmbientColor = vec3( 0.1, 0.1, 0.1 ) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3( 0.3, 0.3, 0.3 );

	vec3 n = normalize( Normal_cameraspace );
	vec3 l = normalize( LightDirection_cameraspace );
	float cosTheta = clamp( dot( n, l ), 0.2, 1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize( EyeDirection_cameraspace );
	// Direction in which the triangle reflects the light
	vec3 R = reflect( -l, n );
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E, R ), 0, 1 );
	
	
	float visibility = 1.0;

	 //variable bias
	 float bias = 0.005 * tan( acos( cosTheta ) );
	 bias = clamp( bias, 0, 0.01 );

	// dFragment to the light
	float dFragment = ( ShadowCoord.z-bias ) / ShadowCoord.w;
	float dBlocker = 0;
	float penumbra = 1;
	float wLight = 5.0;

	if( shadowLevel == 3 ) {
		// Sample the shadow map 8 times
		float count = 0;
		float temp;
		float centerBlocker = texture2D( shadowMap, ShadowCoord.xy).r;
		float scale = (wLight * (dFragment - centerBlocker)) / dFragment;
		for( int i = 0; i < 16; i++ ) {    
			temp = texture2D( shadowMap, ShadowCoord.xy + (scale * poissonDisk( i ) / 50.0) ).r;
			if( temp < dFragment ) {
				dBlocker += temp;
				count += 1;
			}
		}

		if( count > 0 ) {
			dBlocker /= count;
			penumbra = wLight * (dFragment - dBlocker) / dFragment;
		}
	}

	if( shadowLevel == 1 ) {
		if( texture2D( shadowMap,  ShadowCoord.xy).r < dFragment ) {
			visibility -= 0.8;
		}
	} else if( shadowLevel > 1 ) {
		float iterations = 32;
		float sub = 0.8f / iterations;
		for( int i = 0; i < iterations; i++ ) {
			int index = mod( int( 32.0 * random( gl_FragCoord.xyy, i ) ), 32 );
			if( texture2D( shadowMap,  ShadowCoord.xy + (penumbra * poissonDisk( index ) / 250.0) ).r < dFragment ) {
				visibility -= sub;
			}
		}
	}
	visibility = min( visibility, cosTheta );
	MaterialDiffuseColor = vec3( 0.8, 0.8, 0.8 );
	gl_FragColor.rgb = MaterialAmbientColor +
		visibility * MaterialDiffuseColor * LightColor * LightPower +
		visibility * MaterialSpecularColor * LightColor * LightPower * pow( cosAlpha, 5 );

	//gl_FragColor.rgb = MaterialDiffuseColor;
	//gl_FragColor.rgb = vec3( visibility, visibility, visibility );
}