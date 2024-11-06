#pragma once
#include "shape.h"

class  object : public shape
{
public:
	object();
	~object();

	void draw();
	void update();


};
