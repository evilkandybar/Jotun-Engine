#version 120
#extension GL_EXT_gpu_shader4 : enable

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec4 ShadowCoord;
							//10
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform sampler2D shadowMap;

vec2 poissonDisk( int ind ) {
   if( ind == 0 ) {
	return vec2( -0.1094937f, 0.752005 );
   } 
   if( ind == 1 ) {				//20
	return vec2( -0.5059697f, 0.7294227f );
   } 
   if( ind == 2 ) {
	return vec2( -0.3904303f, 0.5678311f );
   }  
   if( ind == 3 ) {
	return vec2( -0.3050305f, 0.7459931f );
   }  
   if( ind == 4 ) {
	return vec2( -0.1725386f, 0.506364f );
   }  
   if( ind == 5 ) {
	return vec2( 0.1979104f, 0.7830779f );
   }  
   if( ind == 6 ) {				//30
	return vec2( 0.0663829f, 0.9336991f );
   }  
   if( ind == 7 ) {
	return vec2( -0.163072f, 0.9741971f );
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
	return vec2( -0.4994766f, 0.4100508f );
   }  
   if( ind == 13 ) {
	return vec2( -0.6305282f, 0.5586912f );
   }  
   if( ind == 14 ) {
	return vec2( -0.5874177f, 0.1295959f );
   }  
   if( ind == 15 ) {
	return vec2( -0.4260757f, -0.02231212f );
   }  
   if( ind == 16 ) {
	return vec2( -0.8381009f, -0.1279669f );
   }  
   if( ind == 17 ) {
	return vec2( -0.8977778f, 0.1717084f );
   }  
   if( ind == 18 ) {
	return vec2( -0.8211543f, 0.365194f );
   }  
   if( ind == 19 ) {
	return vec2( -0.6365152f, -0.229197f );
   }  
   if( ind == 20 ) {
	return vec2( -0.8206947f, -0.3301564f );
   }  
   if( ind == 21 ) {
	return vec2( -0.08938109f, 0.005763604f );
   }  
   if( ind == 22 ) {
	return vec2( -0.3123821f, -0.2344262f );
   }  
   if( ind == 23 ) {
	return vec2( 0.1038207f, 0.2167438f );
   }  
   if( ind == 24 ) {
	return vec2( 0.3256707f, 0.2347208f );
   }  
   if( ind == 25 ) {
	return vec2( 0.3405131f, 0.4458854f );
   }  
   if( ind == 16 ) {
	return vec2( -0.6740047f, -0.4649915f );
   }  
   if( ind == 17 ) {
	return vec2( -0.6670403f, -0.658087f );
   }  
   if( ind == 18 ) {
	return vec2( -0.4680224f, -0.4418066f );
   }  
   if( ind == 19 ) {
	return vec2( 0.09780561f, -0.1236207f );
   }  
   if( ind == 20 ) {
	return vec2( 0.030519f, -0.3487186f );
   }  
   if( ind == 21 ) {
	return vec2( 0.4240496f, -0.1010172f );
   }  
   if( ind == 22 ) {
	return vec2(  0.3915275f, -0.333679f );
   }  
   if( ind == 23 ) {
	return vec2( 0.2238118f, 0.05503683f );
   }  
   if( ind == 24 ) {
	return vec2( -0.1218127f, -0.1943101f );
   }  
   if( ind == 25 ) {
	return vec2( 0.6776094f, 0.5458294f );
   }  
   if( ind == 26 ) {
	return vec2( 0.6469344f, 0.2682991f );
   }  
   if( ind == 27 ) {
	return vec2(  0.4293563f, 0.626775f );
   }  
   if( ind == 28 ) {
	return vec2( 0.4987282f, 0.1368884f );
   }  
   if( ind == 29 ) {
	return vec2( 0.4230149f, 0.8571401f );
   }  
   if( ind == 30 ) {
	return vec2( 0.9180746f, 0.3412085f );
   }  
   if( ind == 31 ) {
	return vec2( 0.5972316f, 0.7636495f );
   }
}

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);			
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}							//60

void main() {
	// Light emission properties
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 1.0f;

	// Material properties
	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3( 0.1, 0.1, 0.1 ) * MaterialDiffuseColor;	//70
	vec3 MaterialSpecularColor = vec3( 0.3, 0.3, 0.3 );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendiular to the triangle -> 0	//80
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n, l ), 0, 1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect( -l, n );
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1			//90
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E, R ), 0, 1 );
	
	float visibility = 1.0;

	 //variable bias
	 float bias = 0.005*tan(acos(cosTheta));		//100
	 bias = clamp(bias, 0, 0.01 );

	// Distance to the light
	float distance = (ShadowCoord.z-bias)/ShadowCoord.w;
	float dBlocker = 0;
	float penumbra = 0;
	float wLight = 5.0;

	// Sample the shadow map 8 times
	int count = 0;
	float temp;
	for( int i = 0; i < 32; i++ ) {
		temp = texture2D( shadowMap,  ShadowCoord.xy + (wLight * poissonDisk( i ) / 500.0) ).r;
		if( temp < distance ) {
			dBlocker += temp;
			count++;
		}
	}
	if( dBlocker != 0 ) {
		dBlocker /= float(count);
		penumbra = wLight * (distance - dBlocker)/distance;
	}

	int iterations = 16;
	float sub = 0.8f / float(iterations);
	for( int i = 0; i < iterations; i++ ) {
		// 0.2 potentially remain, which is quite dark.
		//sample the depth of the shadow map and compare it to the depth of the fragment
		int index =  int( 32 * random( floor( Position_worldspace.xyz * 1000.0 ), i ) ) % 32;
		if( texture2D( shadowMap,  ShadowCoord.xy + (penumbra * poissonDisk(index ) / 250.0) ).r < distance ) {
			visibility -= sub;
		}
	}

	// For spot lights, use either one of these lines instead.
	// if ( texture( shadowMap, (ShadowCoord.xy/ShadowCoord.w) ).z  <  (ShadowCoord.z-bias)/ShadowCoord.w )
	// if ( textureProj( shadowMap, ShadowCoord.xyw ).z  <  (ShadowCoord.z-bias)/ShadowCoord.w )
							//100
	gl_FragColor.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		visibility * MaterialDiffuseColor * LightColor * LightPower * cosTheta+
		// Specular : reflective highlight, like a mirror
		visibility * MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5);
	//gl_FragColor.rgb = vec3( penumbra, penumbra, penumbra );
}
							//110