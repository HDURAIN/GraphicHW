#pragma once

class Timer
{
public:
	Timer();
	float GetDeltaTime();
	void Update();

private:
	float m_LastTime;
	float m_DeltaTime;
};
