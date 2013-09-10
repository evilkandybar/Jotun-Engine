#include "stdafx.h"
#include "Light.h"

int Light::glNameCounter = GL_LIGHT0;

Light::Light(void) : shadowCam( 45, 512, 512, position, Vector3( 0, 0, 0 ) ) {
	glName = glNameCounter;
	glNameCounter++;
	if( glNameCounter > GL_LIGHT7 ) {
		glNameCounter = GL_LIGHT0;
	}
	shadowCam.setFarClip( 100 );
}

Light::Light( float newX, float newY, float newZ, 
			 float newR, float newG, float newB, float newA ) : 
			 position( newX, newY, newZ ), color( newR, newG, newB, newA ),
			 shadowCam( 45, 512, 512, position, Vector3( 0, 0, 0 ) ) {
	glName = glNameCounter;
	glNameCounter++;
	if( glNameCounter > GL_LIGHT7 ) {
		glNameCounter = GL_LIGHT0;
	}
	shadowCam.setFarClip( 100 );
}

Light::~Light(void) {}

void Light::enable() {
	glEnable( glName );
}

void Light::disable() {
	glDisable( glName );
}

void Light::update() {
	float *data = new float[4];
	glLightfv( glName, GL_POSITION, position.toArray( data ) );
	glLightfv( glName, GL_COLOR, color.getData() );
	glLightfv( glName, GL_SPECULAR, color.getData() );
	glLightfv( glName, GL_EMISSION, color.getData() );
}

void Light::drawGizmo() {
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glUseProgram( 0 );
	glColor4f( 1, 1, 1, 1 );
	glBegin( GL_TRIANGLES );
	glVertex3f( position.getX(), position.getY(), position.getZ() );
	glVertex3f( position.getX()-1, position.getY()-1, position.getZ() );
	glVertex3f( position.getX()+1, position.getY()+1, position.getZ() );
	glEnd();
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
}

void Light::setPosition( Vector3 newPos ) {
	position = newPos;
	shadowCam.setPosition( position );
}

void Light::setPosition( float x, float y, float z ) {
	position = Vector3( x, y, z );
	shadowCam.setPosition( position );
}

void Light::setColor( Color newCol ) {
	color = newCol;
}

void Light::setColor( float r, float g, float b, float a ) {
	color = Color( r, g, b, a );
}

void Light::setColor( float *data ) {
	color = Color( data );
}

void Light::prepareShadowCam() {
	shadowCam.setPosition( position );
	//shadowCam.lookAt( Vector3( 0, 0, 0 ) );
	//shadowCam.setActive();
	//shadowCam.prepare();
	shadowCam.setMatricies();
	glViewport( 0, 0, 512, 512 );	//512 by 512 shadow map, for now. Eventually, make a global setting
	glColorMask( 0, 0, 0, 0 );
	glCullFace( GL_FRONT );
	glShadeModel( GL_FLAT );
}

Camera& Light::getCamera() {
	return shadowCam;
}