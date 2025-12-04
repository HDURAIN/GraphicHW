#pragma once
#include <string>

class Log
{
public:
	static void Info(const std::string& message);
	static void Warn(const std::string& message);
	static void Error(const std::string& message);
};
