#include "FileSystem.h"
#include "Log.h"

#include <fstream>
#include <sstream>

std::string FileSystem::ReadFile(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		Log::Error("Failed to open file: " + filePath);
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}
