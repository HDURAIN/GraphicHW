#pragma once
#include <string>
#include <unordered_map>
#include "Texture.h"

class TextureLibrary
{
public:
	// Load or fetch an already-loaded texture
	// Ensures the same path is only loaded once
	static Texture* GetOrLoad(const std::string& path);

	// Optional: manually clear all cached textures (shutdown)
	static void Clear();

private:
	// Prevent instantiation ¡ª this is a pure static resource system
	TextureLibrary() = delete;

private:
	// Cache: path ¡ú Texture*
	static std::unordered_map<std::string, Texture*> s_TextureCache;
};
