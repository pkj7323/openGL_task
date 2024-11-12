#include "pch.h"
#include "TimeManager.h"
TimeManager::TimeManager()
{
	m_currentTime = 0;
	m_deltaTime = 0;
	FPS = 0;
}
TimeManager::~TimeManager()
= default;

void TimeManager::update()
{
	m_deltaTime = glutGet(GLUT_ELAPSED_TIME) / 1000.f - m_currentTime;
	FPS = 1 / m_deltaTime;
	m_currentTime += m_deltaTime;
}



void TimeManager::init()
{
	m_currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	m_deltaTime = 0;
}
