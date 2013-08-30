#pragma once

//Represents a RGBA color
//Because descriptive naming
class Color
{
public:
	Color(void);
	Color( float r, float g, float b );
	Color( float r, float g, float b, float a );
	Color( float* );
	~Color(void);
	float getR() {return data[0];};
	float getG() {return data[1];};
	float getB() {return data[2];};
	float getA() {return data[3];};
	float* getData() {return data;};
protected:
	float *data;
};

