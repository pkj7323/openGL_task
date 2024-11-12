#include "pch.h"
#include "CollisionManager.h"

#include "shape.h"

CollisionManager::CollisionManager()
{
}
CollisionManager::~CollisionManager()
{
}


void CollisionManager::add_collision_pair(const string& group, shape* shape1, shape* shape2)
{
	if (collision_pairs.find(group) == collision_pairs.end())
	{
		collision_pairs[group] = { {},{} };
	}
	if (shape1)
	{
		collision_pairs[group][0].push_back(shape1);
	}
	if (shape2)
	{
		collision_pairs[group][1].push_back(shape2);
	}
}

void CollisionManager::handle_collision()
{
	for (auto& pair : collision_pairs)
	{
		const string& group = pair.first;
		auto& pairs = pair.second;
		for (auto* a : pairs[0])

		{
			for (auto* b : pairs[1])
			{
				if (collide(a, b))
				{
					a->handle_collision(group, b);
					b->handle_collision(group, a);
				}
			}
		}
	}

}

void CollisionManager::remove_collision_object(shape* shape)
{
	for (auto& pair : collision_pairs)
	{
		auto& pairs = pair.second;
		auto& a = pairs[0];
		auto& b = pairs[1];
		a.erase(std::remove(a.begin(), a.end(), shape), a.end());
		b.erase(std::remove(b.begin(), b.end(), shape), b.end());
	}
}

bool CollisionManager::collide(shape* shape1, shape* shape2)
{
	auto a = shape1->get_bb();
	auto b = shape2->get_bb();


	glm::vec2 temp_a= glm::vec2(shape1->GetWorldTrans() * glm::vec4(a.x, a.y, 0, 1.f));
	float left_a = temp_a.x;
	float top_a = temp_a.y;
	temp_a = glm::vec2(shape1->GetWorldTrans() * glm::vec4(a.z, a.w, 0, 1.f));
	float right_a = temp_a.x;
	float bottom_a = temp_a.y;

	glm::vec2 temp_b = glm::vec2(shape2->GetWorldTrans() * glm::vec4(b.x, b.y, 0, 1.f));
	float left_b = temp_b.x;
	float top_b = temp_b.y;
	temp_b = glm::vec2(shape2->GetWorldTrans() * glm::vec4(b.z, b.w, 0, 1.f));
	float right_b = temp_b.x;
	float bottom_b = temp_b.y;

	if (left_a > right_b)
	{
		return false;
	}
	if (right_a < left_b)
	{
		return false;
	}
	if (top_a < bottom_b)
	{
		return false;
	}
	if (bottom_a > top_b)
	{
		return false;
	}

	//°ãÄ¡¸é
	return true;
}
