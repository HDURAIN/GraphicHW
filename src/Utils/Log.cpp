#include "Log.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

static void SetConsoleColor(int level)
{
#ifdef _WIN32
	switch (level)
	{
	case 0: SetConsoleTextAttribute(consoleHandle, 10); break; // Green
	case 1: SetConsoleTextAttribute(consoleHandle, 14); break; // Yellow
	case 2: SetConsoleTextAttribute(consoleHandle, 12); break; // Red
	}
#else
	switch (level)
	{
	case 0: std::cout << "\033[32m"; break;
	case 1: std::cout << "\033[33m"; break;
	case 2: std::cout << "\033[31m"; break;
	}
#endif
}

static void ResetConsoleColor()
{
#ifdef _WIN32
	SetConsoleTextAttribute(consoleHandle, 7);
#else
	std::cout << "\033[0m";
#endif
}

void Log::Info(const std::string& message)
{
	SetConsoleColor(0);
	std::cout << "[INFO] " << message << std::endl;
	ResetConsoleColor();
}

void Log::Warn(const std::string& message)
{
	SetConsoleColor(1);
	std::cout << "[WARN] " << message << std::endl;
	ResetConsoleColor();
}

void Log::Error(const std::string& message)
{
	SetConsoleColor(2);
	std::cout << "[ERROR] " << message << std::endl;
	ResetConsoleColor();
}
