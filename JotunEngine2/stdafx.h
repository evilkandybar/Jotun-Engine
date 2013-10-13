#pragma once
// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw3.h>

// Include GLM
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
using namespace glm;

//useful includes
#include "stb_image.h"
#include "Input.h"

//interfaces
#include "InputHandler.h"
#include "Transformable.h"

#define _CRT_SECURE_NO_WARNINGS 1