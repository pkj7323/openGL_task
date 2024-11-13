#pragma once
#include "shape.h"

class  Bar : public shape
{
	float speed;
	glm::vec2 dir;

	
public:
	Bar();
	~Bar();

	void draw();
	void update();
	void move();
	void handle_collision(const string& group, shape* other) override;

	void plusSpeed(float speed) { this->speed += speed; }
};
