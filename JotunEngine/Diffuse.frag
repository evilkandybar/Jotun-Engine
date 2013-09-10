uniform sampler2D tex;
uniform sampler2D shadowMap;

varying vec4 diffuse, ambient;
varying vec3 normal, halfVector;
varying vec2 texCoord;
varying vec2 grassCoord;
varying vec4 shadowCoord;
 
void main()
{
	//lighting calculations.
    vec4 texel;
    vec3 n,halfV,lightDir;
    float NdotL,NdotHV;
 
    lightDir = vec3(gl_LightSource[0].position);
 
    /* The ambient term will always be present */
    vec4 color = ambient;
    /* a fragment shader can't write a varying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);
    /* compute the dot product between normal and ldir */
 
    NdotL = max(dot(n,lightDir),0.0);
    if (NdotL > 0.0) {
        color += diffuse * NdotL;
        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += gl_FrontMaterial.specular *
                gl_LightSource[0].specular *
                pow(NdotHV, gl_FrontMaterial.shininess);
    }
 
    //gl_FragColor = texture2D( tex, texCoord );

	//shadow calaulations
	vec4 shadowCoordWDivide = shadowCoord / shadowCoord.w;
	//add a slight bias to avoid bleh lines
	shadowCoordWDivide.z += 0.005;
	float distFromLight = texture2D( shadowMap, shadowCoordWDivide.st ).z;
	float shadow = 1.0;
	if( shadowCoord.w > 0.0 ) {
		shadow = distFromLight < shadowCoordWDivide.z ? 0.5 : 1;
	}

	//if( shadow < 0.9 ) {
	//	gl_FragColor = vec4( distFromLight, 0, 0, 1 );
	//} else {
	//	gl_FragColor = vec4( 0, distFromLight, 0, 1 );
	//}
    gl_FragColor = vec4( shadowCoordWDivide.r, 0, shadowCoordWDivide.b, 1 );// * distFromLight;// * color;
}