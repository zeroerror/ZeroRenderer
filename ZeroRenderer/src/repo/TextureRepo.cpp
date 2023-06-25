#include "TextureRepo.h"
#include "Database.h"

TextureRepo::TextureRepo() {
	allTextures = std::unordered_map<std::string, Texture*>();
}

TextureRepo::~TextureRepo() {
	std::cout << "TextureRepo::~TextureRepo() " << std::endl;
	for (auto& texture : allTextures) {
		texture.second->~Texture();
	}
}

std::string TextureRepo::LoadTexture(const char* path) {
	Texture* texture = new Texture(path);
	std::string guid = Database::GetGUIDFromAssetPath(path);
	allTextures.insert(std::pair<std::string, Texture*>(guid, texture));
	std::cout << "TextureRepo::LoadTexture **** path - " << path << " GUID - " << guid << std::endl;
	return guid;
}

Texture* TextureRepo::GetTextureByGUID(const std::string& guid) {
	std::unordered_map<std::string, Texture*>::iterator it = allTextures.find(guid);
	if (it != allTextures.end()) {
		return allTextures.at(guid);
	}

	std::cout << "  ################ GetTextureByGUID: " << guid << " not found" << std::endl;
	return nullptr;
}