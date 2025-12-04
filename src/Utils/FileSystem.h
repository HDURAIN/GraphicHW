#pragma once
#include <string>

class FileSystem
{
public:
	static std::string ReadFile(const std::string& filePath);
};
