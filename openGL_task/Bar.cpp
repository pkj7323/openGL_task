#include "pch.h"
#include "Bar.h"

#include "TimeManager.h"

Bar::Bar() : speed{ 0.5f }, dir{ glm::vec2(1,0) }
{
	auto model = GetModel();
	model->vertices.emplace_back(0.5f, -0.7f, 0.0f);
	model->vertices.emplace_back(-0.5f, -0.7f, 0.0f);
	model->vertices.emplace_back(-0.5f, -0.75f, 0.0f);
	model->vertices.emplace_back(0.5f, -0.75f, 0.0f);


	for (int i = 0; i < model->vertices.size(); i++)
	{
		model->colors.emplace_back(0,0.5,1);
	}

	model->faces.emplace_back(0, 1, 2);
	model->faces.emplace_back(0, 2, 3);
	CalculateSize();
	UpdateBuffer();
}

Bar::~Bar()
{
}

void Bar::draw()
{
	shape::draw();
}

void Bar::update()
{
	shape::update();
	move();
	if(GetModel()->vertices[0].x >= 1.0f)
	{
		dir.x = -1;
	}
	else if (GetModel()->vertices[1].x <= -1.0f)
	{
		dir.x = 1;
	}
}

void Bar::move()
{
	auto model = GetModel();
	SetOffset(glm::vec2(dir.x * speed * DT, dir.y * speed * DT));
	for (auto& v : model->vertices)
	{
		v.x += dir.x * speed * DT;
	}
	UpdateBuffer();
}

void Bar::handle_collision(const string& group, shape* other)
{
	if (group == "Bar:Object")
	{
		//cout << "Bar collided with object" << endl;
	}
}
