#pragma once
#include <string>
#include <vector>

class FileSystem
{
public:
	// Read entire file content as string
	static std::string ReadFile(const std::string& filePath);

	// NEW in TASK 7:
	// List all file names under a directory (e.g., /assets/textures/)
	// Should return only actual file names (e.g., "brick.jpg", "metal.jpg")
	static std::vector<std::string> ListFiles(const std::string& directory);
};
