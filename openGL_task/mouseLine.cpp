#include "pch.h"
#include "mouseLine.h"

mouseLine::mouseLine()
{
	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
	isClick = false;
}

mouseLine::~mouseLine()
{
}

void mouseLine::draw()
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void mouseLine::update()
{
}

void mouseLine::click(float x1, float y1, float x2, float y2)
{
	isClick = true;
	setP1(x1, y1);
	setP2(x2, y2);
}

void mouseLine::setP1(float x1, float y1)
{
	this->x1 = x1;
	this->y1 = y1;
}

void mouseLine::setP1(glm::vec2 p1)
{
	this->x1 = p1.x;
	this->y1 = p1.y;
}

void mouseLine::setP2(float x2, float y2)
{
	this->x2 = x2;
	this->y2 = y2;
}

void mouseLine::setP2(glm::vec2 p2)
{
	this->x2 = p2.x;
	this->y2 = p2.y;
}

void mouseLine::collisionCheck()
{
}

void mouseLine::drag(float xpos, float ypos)
{
	if (isClick)
	{
		setP2(xpos, ypos);
	}
}
