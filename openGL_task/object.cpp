#include "pch.h"
#include "object.h"

object::object() 
{
	std::default_random_engine dre{ std::random_device{}() };
	std::uniform_real_distribution<float> colorRd{ 0.f, 1.f };
	auto model = GetModel();
	model->vertices.emplace_back(0.5f, 0.5f, 0.0f);
	model->vertices.emplace_back(-0.5f, 0.5f, 0.0f);
	model->vertices.emplace_back(-0.5f, -0.5f, 0.0f);
	model->vertices.emplace_back(0.5f, -0.5f, 0.0f);


	for (int i = 0;i < model->vertices.size(); i++)
	{
		model->colors.emplace_back(colorRd(dre),colorRd(dre),colorRd(dre));
	}

	model->faces.emplace_back(0, 1, 2);
	model->faces.emplace_back(0, 2, 3);



	UpdateBuffer();
}

object::~object()
{
}

void object::draw()
{
	shape::draw();
}

void object::update()
{
	shape::update();
}
