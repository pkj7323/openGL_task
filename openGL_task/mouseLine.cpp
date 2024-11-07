#include "pch.h"
#include "mouseLine.h"

#include "object.h"

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
	glUniformMatrix4fv(glGetUniformLocation(Shader::Instance()->GetID(), "world")
		, 1, GL_FALSE
		, glm::value_ptr(glm::mat4(1.0f)));
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

glm::vec2 mouseLine::intersection_point(glm::vec2 lineStart, glm::vec2 lineEnd, glm::vec2 p1, glm::vec2 p2)
{
	glm::vec2 a1 = lineStart;
	glm::vec2 b1 = lineEnd;

	glm::vec2 a2 = p1;
	glm::vec2 b2 = p2;

	auto A1 = b1.y - a1.y;
	auto B1 = a1.x - b1.x;
	auto C1 = A1 * a1.x + B1 * a1.y;

	auto A2 = b2.y - a2.y;
	auto B2 = a2.x - b2.x;
	auto C2 = A2 * a2.x + B2 * a2.y;

	auto determinant = A1 * B2 - A2 * B1;

	if (determinant == 0)
		return glm::vec2(INFINITE,INFINITE);

	auto x = (C1 * B2 - C2 * B1) / determinant;
	auto y = (A1 * C2 - A2 * C1) / determinant;
	return glm::vec2(x, y);
}

bool mouseLine::isCCW(vector<glm::vec3> vecs)
{
	float sum = 0;
	if (vecs.size() < 3)
	{
		return false;
	}
	auto p0 = glm::vec2(vecs[0]);
	auto p1 = glm::vec2(vecs[1]);
	auto p2 = glm::vec2(vecs[2]);

	auto v1 = p1 - p0;
	auto v2 = p2 - p0;
	glm::mat2x2 mat(v1.x, v1.y, v2.x, v2.y);
	sum += glm::determinant(mat);
	return sum > 0;
}

void mouseLine::collisionCheck(vector<object*>& objects)
{
    vector<intersection> intersect_points;

    for (auto it = objects.begin(); it != objects.end();)
    {
        object* obj = *it;
        intersect_points.clear();

        for (size_t i = 0; i < obj->GetModel()->vertices.size(); i++)
        {
            auto p1 = glm::vec2(obj->GetModel()->vertices[i]);
            auto p2 = glm::vec2(obj->GetModel()->vertices[(i + 1) % obj->GetModel()->vertices.size()]);
            if (math::intersect(glm::vec2(x1, y1), glm::vec2(x2, y2), p1, p2))
            {
                glm::vec2 intersect_new_vertex = intersection_point(glm::vec2(x1, y1), glm::vec2(x2, y2), p1, p2);
                if (intersect_new_vertex != glm::vec2(INFINITE, INFINITE))
                {
                    intersect_points.emplace_back(intersect_new_vertex, i, (i + 1) % obj->GetModel()->vertices.size());
					cout << intersect_new_vertex.x << "," << intersect_new_vertex.y << endl;
					cout << i << "," << (i + 1) % obj->GetModel()->vertices.size() << endl;
                }

            }
        }
		//intersect_points y값기준 정렬 필요?
        if (intersect_points.size() >= 2)
        {
            vector<glm::vec3> new_vertices1, new_vertices2;
            
            
			bool isDownToUP = intersect_points.front().point.y < intersect_points.back().point.y;
            // Create first polygon
            for (int i = 0; i < obj->GetModel()->vertices.size(); ++i)
            {
                new_vertices1.push_back(obj->GetModel()->vertices[i]);
				if (i == intersect_points.front().index1)
				{
					new_vertices1.emplace_back(intersect_points.front().point, 0.0f);
					new_vertices1.emplace_back(intersect_points.back().point, 0.0f);
					for (int j = intersect_points.back().index2; j != intersect_points.front().index1; 
						j = (j + 1) % obj->GetModel()->vertices.size())
					{
						new_vertices1.push_back(obj->GetModel()->vertices[j]);
					}
					break;
				}
				else if (i == intersect_points.back().index1)
				{
					new_vertices1.emplace_back(intersect_points.back().point, 0.0f);
					new_vertices1.emplace_back(intersect_points.front().point, 0.0f);
					for (int j = intersect_points.front().index2; j != intersect_points.back().index1; 
						j = (j + 1) % obj->GetModel()->vertices.size())
					{
						new_vertices1.push_back(obj->GetModel()->vertices[j]);
					}
					break;
				}
				
            }
			
				new_vertices2.emplace_back(intersect_points.front().point, 0.0f);
				for (int j = intersect_points.front().index2; j != intersect_points.back().index2;
					j = (j + 1) % obj->GetModel()->vertices.size())
				{
					new_vertices2.emplace_back(obj->GetModel()->vertices[j]);
				}
				new_vertices2.emplace_back(intersect_points.back().point, 0.0f);
			


			// Ensure both polygons are CCW
			if (!isCCW(new_vertices1))
			{
				std::reverse(new_vertices1.begin(), new_vertices1.end());
			}
			if (!isCCW(new_vertices2))
			{
				std::reverse(new_vertices2.begin(), new_vertices2.end());
			}
			
			{
				std::default_random_engine dre{ std::random_device{}() };
				std::uniform_real_distribution<float> colorRd{ 0.f, 1.f };
				// Update the object with the first polygon
				obj->GetModel()->vertices = new_vertices1;
				obj->GetModel()->faces.clear();
				obj->GetModel()->colors.clear();
				for (int i = 0; i < new_vertices1.size(); ++i)
				{
					obj->GetModel()->colors.emplace_back(colorRd(dre), colorRd(dre), colorRd(dre));
				}
				for (int i = 0; i < new_vertices1.size() - 1; ++i)
				{
					obj->GetModel()->faces.emplace_back(0, (i + 1) % new_vertices1.size(), (i + 2) % new_vertices1.size());
				}

				obj->UpdateBuffer();

				// Create a new object for the second polygon
				object* new_obj = new object();
				new_obj->GetModel()->vertices = new_vertices2;
				new_obj->GetModel()->faces.clear();
				new_obj->GetModel()->colors.clear();
				for (int i = 0; i < new_vertices1.size(); ++i)
				{
					new_obj->GetModel()->colors.emplace_back(colorRd(dre), colorRd(dre), colorRd(dre));
				}
				for (int i = 0; i < new_vertices1.size() - 1; ++i)
				{
					new_obj->GetModel()->faces.emplace_back(0, (i + 1) % new_vertices1.size(), (i + 2) % new_vertices1.size());
				}
				new_obj->UpdateBuffer();
				objects.push_back(new_obj);
			}
			
			break;
            
        }
        else
        {
            ++it;
        }

    }
}

void mouseLine::drag(float xpos, float ypos)
{
	if (isClick)
	{
		setP2(xpos, ypos);
	}
}
