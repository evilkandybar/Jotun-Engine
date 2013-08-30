#include "stdafx.h"
#include "Color.h"

Color::Color(void) {
	data = new float[4];
	for( int i = 0; i < 4; i++ ) {
		data[i] = 0;
	}
}

Color::Color( float r, float g, float b ) {
	data = new float[4];
	data[0] = r;
	data[1] = g;
	data[2] = b;
	data[3] = 0;
}

Color::Color( float r, float g, float b, float a ) {
	data = new float[4];
	data[0] = r;
	data[1] = g;
	data[2] = b;
	data[3] = a;
}

//copy by value so we can modify it later
Color::Color( float *newData ) {
	data = new float[4];
	for( int i = 0; i < 4; i++ ) {
		data[i] = newData[i];
	}
}

Color::~Color(void) {
	delete[] data;
}