varying vec4 diffuse, ambient;
varying vec3 normal, halfVector;
varying vec2 texCoord;
varying vec4 shadowCoord;

uniform vec4 diffuseUpload;

void main()
{
	shadowCoord = gl_TextureMatrix[7] * gl_Vertex;
    texCoord = gl_MultiTexCoord0.st;
    /* first transform the normal into eye space and
    normalize the result */
    normal = normalize(gl_NormalMatrix * gl_Normal);
 
    /* pass the halfVector to the fragment shader */
    halfVector = gl_LightSource[0].halfVector.xyz;
 
    /* Compute the diffuse, ambient and globalAmbient terms */
    diffuse = diffuseUpload * gl_LightSource[0].diffuse;
	ambient = diffuseUpload * gl_LightSource[0].ambient;
    ambient += diffuseUpload * gl_LightModel.ambient;
    gl_Position = ftransform();
}