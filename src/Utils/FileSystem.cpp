#include "FileSystem.h"
#include "Log.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <Shlwapi.h>   // PathFileExistsA
#pragma comment(lib, "Shlwapi.lib")
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

// ------------------------------------------------------------
// Read entire file into string
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Normalize directory path (ensure trailing slash)
// ------------------------------------------------------------
static std::string NormalizeDirectory(const std::string& dir)
{
	if (dir.empty())
		return "./";

	std::string d = dir;

#ifdef _WIN32
	if (d.back() != '\\')
		d += "\\";
#else
	if (d.back() != '/')
		d += "/";
#endif

	return d;
}

// ------------------------------------------------------------
// Check if directory exists
// ------------------------------------------------------------
static bool DirectoryExists(const std::string& path)
{
#ifdef _WIN32
	return PathFileExistsA(path.c_str());
#else
	struct stat s;
	return (stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR));
#endif
}

// ------------------------------------------------------------
// Robust, cross-platform, correct working directory handling
// ------------------------------------------------------------
std::vector<std::string> FileSystem::ListFiles(const std::string& rawDirectory)
{
	std::vector<std::string> result;

	// Normalize path (ensure trailing slash)
	std::string directory = NormalizeDirectory(rawDirectory);

	// Try relative path: ./assets/textures/
	std::string relativePath = "./" + directory;

	// If relative exists ¡ú use it
	std::string finalPath = DirectoryExists(relativePath) ? relativePath : directory;

	if (!DirectoryExists(finalPath))
	{
		Log::Warn("FileSystem::ListFiles - directory not found: " + finalPath);
		return result;
	}

#ifdef _WIN32
	// Windows FindFirstFile requires wildcard pattern
	std::string searchPattern = finalPath + "*.jpg";

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		Log::Warn("FileSystem::ListFiles - no .jpg files found in: " + finalPath);
		return result;
	}

	do
	{
		// Skip directories
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			result.push_back(findData.cFileName);

	} while (FindNextFileA(hFind, &findData));

	FindClose(hFind);

#else
	DIR* dir = opendir(finalPath.c_str());
	if (!dir)
	{
		Log::Warn("FileSystem::ListFiles - cannot open: " + finalPath);
		return result;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr)
	{
		std::string name = entry->d_name;

		// Accept only .jpg
		if (name.size() > 4 &&
			name.substr(name.size() - 4) == ".jpg")
		{
			result.push_back(name);
		}
	}

	closedir(dir);
#endif

	// Optional: sort alphabetically
	std::sort(result.begin(), result.end());

	return result;
}
