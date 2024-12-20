﻿#pragma once
#include "shape.h"

class  object : public shape
{
public:
	//TODO:복사 생성자 만들기
	object(const object& obj);
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
	void SetSpeed(glm::vec2 speed) { m_speed = speed; }
	glm::vec2 GetSpeed() { return m_speed; }
	void SetGravityScale(float scalar) { m_Gravity *= scalar; }
	void StopMove(bool stop) { stopMove = stop; }

	void handle_collision(const string& group, shape* other) override;

	bool GetIsKilled() { return isKilled; }

	void onBarMove();

	void plusSpeed(float speed) { m_speed += speed; }
private:
	
	glm::vec2 m_dir;
	glm::vec2 m_speed;
	float m_theta;
	bool on_trace = false;
	float m_Gravity;
	bool stopMove = false;
	bool onKillTimer = false;
	float killTimer;
	bool isKilled=false;
	bool onBar = false;
};
