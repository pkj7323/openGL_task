#pragma once
#include "shape.h"

class  Bar : public shape
{
public:
	Bar();
	~Bar();

	void draw();
	void update();

	void handle_collision(const string& group, shape* other) override;
};
