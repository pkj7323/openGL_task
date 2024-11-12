#pragma once
class shape;
class CollisionManager
{
	SINGLE(CollisionManager)
public:
	void add_collision_pair(const string& group, shape* shape1, shape* shape2);
	void handle_collision();
	void remove_collision_object(shape* shape);
	bool collide(shape* shape1, shape* shape2);
private:
	std::map<std::string, std::vector<std::vector<shape*>>> collision_pairs = {
		{"key", std::vector<std::vector<shape*>>(2)}
	};
};

