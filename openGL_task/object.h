#pragma once
#include "shape.h"

class  object : public shape
{
public:
	object();
	~object();

	void draw();
	void update();

	void move();

	bool is_out();
	void SetDir(glm::vec2 dir) { m_dir = dir; }
	glm::vec2 GetDir() { return m_dir; }

	void onTrace(bool on_off) { on_trace = on_off; }
	bool isOnTrace() { return on_trace; }
	void SetSpeed(float speed) { m_speed = speed; }
	float GetSpeed() { return m_speed; }
	void SetGravityScale(float scalar) { m_Gravity *= scalar; }
private:
	glm::vec2 m_dir;
	float m_speed;
	bool on_trace = false;
	float m_Gravity;

};
