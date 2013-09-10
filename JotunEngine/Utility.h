#ifndef UTILITY_H
#define UTILITY_H
#include <assimp/scene.h>

aiMatrix4x4 floatsToMatrix( float* data );
float* matrixToFloats( aiMatrix4x4 data );

#endif