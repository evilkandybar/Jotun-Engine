#pragma once

//A thing, anything, that can be rendered
class RenderObject
{
public:
	RenderObject(void);
	~RenderObject(void);
	virtual void draw() {};
};

