#include "pch.h"
#include "object.h"
#include "CollisionManager.h"
#include "TimeManager.h"

object::object(const object& other)
	: 
	m_dir(other.m_dir),
	m_speed(other.m_speed),
	on_trace(other.on_trace),
	m_Gravity(other.m_Gravity),
	stopMove(other.stopMove),
	onKillTimer(other.onKillTimer),
	killTimer(other.killTimer),
	isKilled(other.isKilled),
	onBar(other.onBar)
{
}
object::object() : m_dir{ glm::vec2(1,0) }, m_speed{ 0.5f }, m_Gravity{ Gravity / 20.f }
{
	
	std::default_random_engine dre{ std::random_device{}() };
	std::uniform_real_distribution<float> colorRd{ 0.f, 1.f };
	std::uniform_int_distribution<int> shapeRd{ 1, 4 };
	auto model = GetModel();
	switch (shapeRd(dre))
	{
	case 1:
		{
			model->vertices.emplace_back(0.25f, 0.25f, 0.0f);
			model->vertices.emplace_back(-0.25f, 0.25f, 0.0f);
			model->vertices.emplace_back(-0.25f, -0.25f, 0.0f);
			model->vertices.emplace_back(0.25f, -0.25f, 0.0f);

			

			for (int i = 0;i < model->vertices.size(); i++)
			{
				model->colors.emplace_back(colorRd(dre),colorRd(dre),colorRd(dre));
			}

			model->faces.emplace_back(0, 1, 2);
			model->faces.emplace_back(0, 2, 3);
			CalculateSize();
			
		}
		break;
	case 2:
		{
			float size = 0.25f;
			model->vertices.emplace_back(size * cos(0.0f), size * sin(0.0f), 0.0f);
			model->vertices.emplace_back(size * cos(2.0f * PI / 5.0f), size * sin(2.0f * PI / 5.0f), 0.0f);
			model->vertices.emplace_back(size * cos(4.0f * PI / 5.0f), size * sin(4.0f * PI / 5.0f), 0.0f);
			model->vertices.emplace_back(size * cos(6.0f * PI / 5.0f), size * sin(6.0f * PI / 5.0f), 0.0f);
			model->vertices.emplace_back(size * cos(8.0f * PI / 5.0f), size * sin(8.0f * PI / 5.0f), 0.0f);

			for (int i = 0; i < model->vertices.size(); i++)
			{
				model->colors.emplace_back(colorRd(dre), colorRd(dre), colorRd(dre));
			}

			// 오각형 면 정의
			model->faces.emplace_back(0, 1, 2);
			model->faces.emplace_back(0, 2, 3);
			model->faces.emplace_back(0, 3, 4);
			CalculateSize();
			
		}
		break;
	case 3:
		{
			float size = 0.25f;
			model->vertices.emplace_back(size * cos(0.0f), size * sin(0.0f), 0.0f);
			model->vertices.emplace_back(size * cos(2.0f * PI / 3.0f), size * sin(2.0f * PI / 3.0f), 0.0f);
			model->vertices.emplace_back(size * cos(4.0f * PI / 3.0f), size * sin(4.0f * PI / 3.0f), 0.0f);

			for (int i = 0; i < model->vertices.size(); i++)
			{
				model->colors.emplace_back(colorRd(dre), colorRd(dre), colorRd(dre));
			}

			// 삼각형 면 정의
			model->faces.emplace_back(0, 1, 2);
			
			CalculateSize();
		}
		break;
	case 4:
		{
			// 육각형 정의
			float size = 0.25f;
			for (int i = 0; i < 6; ++i)
			{
				model->vertices.emplace_back(size * cos(i * PI / 3.0f), size * sin(i * PI / 3.0f), 0.0f);
			}

			for (int i = 0; i < model->vertices.size(); i++)
			{
				model->colors.emplace_back(colorRd(dre), colorRd(dre), colorRd(dre));
			}

			// 육각형 면 정의
			model->faces.emplace_back(0, 1, 2);
			model->faces.emplace_back(0, 2, 3);
			model->faces.emplace_back(0, 3, 4);
			model->faces.emplace_back(0, 4, 5);
			
			
			CalculateSize();
		}
		break;
	default:
		break;
	}
    



	UpdateBuffer();
}

object::~object()
{
}

void object::draw()
{
    shape::draw();

    if (on_trace)
    {
        glBindVertexArray(GetVAO());

        glm::vec3 v{};
        auto model = GetModel();
        for (auto vertex : model->vertices)
        {
            v += vertex;
        }
        v /= model->vertices.size();
        auto dir = glm::normalize(m_dir);

        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 0.0f, 1.0f);
        auto speed = m_speed;
        float theta = m_theta;
        
		int i = 1;
		CalculateCenter();
        while (v.x > -1.5f && v.x < 1.5f && v.y > -1.5f && v.y < 1.5f) // 화면 끝까지 그립니다.
        {
			
			float t = i * 0.1f; // 시간 단위로 증가
			float rotate_x = v.x * glm::cos(glm::radians(theta * t )) - v.y * glm::sin(glm::radians(theta * t ));
			float rotate_y = v.x * glm::sin(glm::radians(theta * t )) + v.y * glm::cos(glm::radians(theta * t ));

			v.x = rotate_x + dir.x * t * speed.x;
			v.y = rotate_y - (speed.y * t + 0.5f * m_Gravity * t * t);

			glVertex2f(v.x, v.y);
			i++;
        }
        glEnd();

        glBindVertexArray(0);
    }
}

void object::update()
{
	if (!stopMove)
	{
		if (onBar)
		{
			onBarMove();
		}else
		{
			move();
		}
	}
	if(onKillTimer)
	{
		killTimer += DT;
		if (killTimer >= 2.0f)
		{
			CollisionManager::Instance()->remove_collision_object(this);
			isKilled = true;
			
		}
	}
	shape::update();
	
}

void object::move()
{
	auto model = GetModel();
	if (m_dir == glm::vec2(0,0))
	{
		for (auto& vertex : model->vertices)
		{
			vertex.y -= m_Gravity * DT;
		}
		UpdateBuffer();
		return;
	}
	m_dir = glm::normalize(m_dir);
	glm::vec3 dir = glm::vec3(m_dir, 0.0f);
	CalculateCenter();
	auto center = GetCenter();
	float theta = 0.05f;
	for (auto& vertex : model->vertices)
	{
		
		auto rotate_x = vertex.x;
		auto rotate_y = vertex.y;

		rotate_x = vertex.x * glm::cos(glm::radians(theta)) - vertex.y * glm::sin(glm::radians(theta));
		rotate_y = vertex.x * glm::sin(glm::radians(theta)) + vertex.y * glm::cos(glm::radians(theta));


		vertex.x = rotate_x;
		vertex.y = rotate_y;

		vertex.x += dir.x * DT * m_speed.x;
		vertex.y -= m_speed.y * DT;
		m_speed.y += m_Gravity * DT;
	}
	m_theta += theta;

	UpdateBuffer();
}

bool object::is_out()
{
	auto model = GetModel();
	vector<bool> outCheck;
	outCheck.resize(model->vertices.size(),false);
	for (int i = 0; i < model->vertices.size();++i)
	{
		auto worldMatrix = GetWorldTrans();
		auto vertex = glm::vec3(worldMatrix * glm::vec4(model->vertices[i],1.0f));
		if (vertex.x > 1.5f || vertex.x < -1.5f || vertex.y > 1.5f || vertex.y < -1.5f)
		{
			outCheck[i] = true;
		}
	}
	bool isOut = false;
	for (int i = 0; i < outCheck.size(); ++i)
	{
		outCheck[(i + 1) % outCheck.size()] = outCheck[i] && outCheck[(i + 1) % outCheck.size()];
	}
	isOut = outCheck[0];
	
	return isOut;
}

void object::handle_collision(const string& group, shape* other)
{
	if (group == "Bar:Object")
	{
		SetOffset(other->GetOffset());
		onBar = true;
		onKillTimer = true;
		this->m_Gravity = 0;
		onTrace(false);
	}else
	{
		SetOffset(glm::vec2(0, 0));
		onBar = false;
		onKillTimer = false;
		this->m_Gravity = Gravity / 20.f;

	}
}

void object::onBarMove()
{
	
	for (auto& vertex : GetModel()->vertices)
	{
		vertex.x += GetOffset().x;
		vertex.y += GetOffset().y;
	}


	UpdateBuffer();
}
