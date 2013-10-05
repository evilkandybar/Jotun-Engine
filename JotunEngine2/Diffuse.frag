#version 120

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
      return vec2( -0.94201624, -0.39906216 ); 
   } else if( ind == 1 ) {				//20
      return vec2( 0.94558609, -0.76890725 );	
   } else if( ind == 2 ) {
      return vec2( -0.094184101, -0.92938870 );			
   } else if( ind == 3 ) {
      return vec2( 0.34495938, 0.29387760 );			
   } else if( ind == 4 ) {
      return vec2( -0.91588581, 0.45771432 ); 			
   } else if( ind == 5 ) {
      return vec2( -0.81544232, -0.87912464 );			
   } else if( ind == 6 ) {				//30
      return vec2( -0.38277543, 0.27676845 );			
   } else if( ind == 7 ) {
      return vec2( 0.97484398, 0.75648379 );			
   } else if( ind == 8 ) {
      return vec2( 0.44323325, -0.97511554 );			
   } else if( ind == 9 ) {
      return vec2( 0.53742981, -0.47373420 );			
   } else if( ind == 10 ) {
      return vec2( -0.26496911, -0.41893023 ); 			
   } else if( ind == 11 ) {				//40
      return vec2( 0.79197514, 0.19090188 );			
   } else if( ind == 12 ) {
      return vec2( -0.24188840, 0.99706507 );			
   } else if( ind == 13 ) {
      return vec2( -0.81409955, 0.91437590 ); 			
   } else if( ind == 14 ) {
      return vec2( 0.19984126, 0.78641367 );			
   } else if( ind == 15 ) {
      return vec2( 0.14383161, -0.14100790 ); 		//50
   }	
}


// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);			
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}							//60

void main(){

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

	// Fixed bias, or...
	//float bias = 0.005;

	// ...variable bias
	 float bias = 0.005*tan(acos(cosTheta));		//100
	 bias = clamp(bias, 0, 0.01 );

	// Distance to the light
	float distance = (ShadowCoord.z-bias)/ShadowCoord.w;
	float dBlocker = 0;

	// Sample the shadow map 8 times
	int index = 0;
	for( int i = 0; i < 8; i++ ) {
		dBlocker += texture2D( shadowMap,  ShadowCoord.xy + poissonDisk( index ) / 100.0 ).r;	//110
	}
	dBlocker /= 8;

	float penumbra = 5 * (distance - dBlocker)/distance;

	float shadowDepth;
	for( int i = 0; i < 8; i++ ) {
		
		// being fully in the shadow will eat up 16*0.05 = 0.8
		// 0.2 potentially remain, which is quite dark.

		//sample the depth of the shadow map
		//compare it to the depth of the fragment
		if( texture2D( shadowMap,  ShadowCoord.xy + (penumbra * poissonDisk( i )) / 700.0 ).r < distance ) {
			visibility -= 0.1;
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