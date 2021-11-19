#pragma once

#define TEXTURE_PATH "\\Resources\\Textures\\"

#include <filesystem>

#include "GameData/ResourceManager.h"

class TextureManager : public ResourceManager<TextureManager, sf::Texture> {
public:
	TextureManager() : ResourceManager() {}

	std::unique_ptr<sf::Texture> Load(const std::string& Path) {
		std::unique_ptr<sf::Texture> Texture = std::make_unique<sf::Texture>();
		if (!Texture->loadFromFile(std::filesystem::current_path().string() + std::string(TEXTURE_PATH) + Path))
		{
			throw ResourceException("Failed to load texture: " + Path);
		}
		return Texture;
	}
};