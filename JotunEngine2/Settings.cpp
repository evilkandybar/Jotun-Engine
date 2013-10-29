#include "Settings.h"

int Settings::shadowQuality = 1;
int Settings::multisamples = 4;

void Settings::setShadowQuality( int newQuality ) {
	shadowQuality = newQuality;
	//find some way to change this in the shaders
}

int Settings::getShadowQuality() {
	return shadowQuality;
}

void Settings::setMultisamples( int samples ) {
	multisamples = samples;
}

int Settings::getMultisamples() {
	return multisamples;
}