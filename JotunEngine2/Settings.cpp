#include "Settings.h"

int Settings::shadowQuality = 3;

void Settings::setShadowQuality( int newQuality ) {
	shadowQuality = newQuality;
	//find some way to change this in the shaders
}

int Settings::getShadowQuality() {
	return shadowQuality;
}