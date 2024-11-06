#pragma once
class mouseLine
{
	float x1, y1;
	float x2, y2;
	bool isClick;
public:
	mouseLine();
	~mouseLine();

	void draw();
	void update();

	void click(float x1, float y1, float x2, float y2);
	void setP1(float x1, float y1);
	void setP1(glm::vec2 p1);
	void setP2(float x2, float y2);
	void setP2(glm::vec2 p2);

	glm::vec2 getP1() const { return glm::vec2(x1, y1); }
	glm::vec2 getP2() const { return glm::vec2(x2, y2); }


	void collisionCheck();
	bool isClicked() { return this->isClick; }
	void drag(float xpos, float ypos);
};
