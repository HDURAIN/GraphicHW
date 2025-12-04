#include "TextureLibrary.h"
#include <iostream>

// Allocate static container
std::unordered_map<std::string, Texture*> TextureLibrary::s_TextureCache;

// ------------------------------------------------------------
// GetOrLoad: return existing texture or load a new one
// ------------------------------------------------------------
Texture* TextureLibrary::GetOrLoad(const std::string& path)
{
	// Already loaded?
	auto it = s_TextureCache.find(path);
	if (it != s_TextureCache.end())
	{
		return it->second;
	}

	// Load new texture
	Texture* tex = nullptr;
	try
	{
		tex = new Texture(path);
	}
	catch (...)
	{
		std::cerr << "[TextureLibrary] Failed to load texture: " << path << std::endl;
		return nullptr;
	}

	s_TextureCache[path] = tex;
	return tex;
}

// ------------------------------------------------------------
// Clear all cached textures (called during engine shutdown)
// ------------------------------------------------------------
void TextureLibrary::Clear()
{
	for (auto& entry : s_TextureCache)
	{
		delete entry.second;
	}
	s_TextureCache.clear();
}
