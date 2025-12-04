#pragma once

class Input
{
public:
	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int button);
	static void GetMousePosition(float& xpos, float& ypos);
};
