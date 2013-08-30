#include "stdafx.h"
#include "Utility.h"

aiMatrix4x4 floatsToMatrix( float* data ) {
	return aiMatrix4x4( data[0], data[1], data[2], data[3],
		data[4], data[5], data[6], data[7], 
		data[8], data[9], data[10], data[11], 
		data[12], data[13], data[14], data[15] );
}

float* matrixToFloats( aiMatrix4x4 data ) {
	float *fData = new float[16];
	for( int i = 0; i < 16; i++ ) {
		fData[i] = *data[i];
	}
	return fData;
}