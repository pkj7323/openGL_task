#pragma once
class TimeManager
{
	SINGLE(TimeManager)
	
public:
	void update();

	void init();
	float GetDeltaTime() const { return m_deltaTime; }
	
private:
	float m_deltaTime;
	float m_currentTime;
	float FPS;
};

