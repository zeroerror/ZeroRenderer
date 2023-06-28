#include "TextureRepo.h"
#include "Database.h"

TextureRepo::TextureRepo() {
	allTextures_sortedByPath = std::unordered_map<std::string, Texture*>();
	allTextures_sortedByGUID = std::unordered_map<std::string, Texture*>();
}

TextureRepo::~TextureRepo() {
	std::cout << "TextureRepo::~TextureRepo() " << std::endl;
	for (auto& texture : allTextures_sortedByPath) {
		texture.second->~Texture();
	}
	for (auto& texture : allTextures_sortedByGUID) {
		texture.second->~Texture();
	}
}

void TextureRepo::LoadTextureByPath(const string& path, Texture*& texture) {
	string guid = Database::GetGUIDFromAssetPath(path);
	if (guid == "") {
		std::cout << "############## TextureRepo::LoadTextureByPath -  path doesn't exist : " << path << std::endl;
		return;
	}

	if (!TryGetTextureByGUID(guid, texture)) {
		texture = new Texture(path);
	}

	allTextures_sortedByGUID.insert(std::pair<std::string, Texture*>(guid, texture));
	allTextures_sortedByPath.insert(std::pair<std::string, Texture*>(path, texture));
	std::cout << "TextureRepo::LoadTexture. Path - " << path << " GUID - " << guid << std::endl;
	return;
}

void TextureRepo::LoadTextureByGUID(const string& guid, Texture*& texture) {
	string path = Database::GetAssetPathFromGUID(guid);
	if (path == "") {
		std::cout << "############## TextureRepo::LoadTextureByGUID -  guid doesn't exist : " << guid << std::endl;
		return;
	}

	if (!TryGetTextureByGUID(guid, texture)) {
		texture = new Texture(path);
	}

	allTextures_sortedByGUID.insert(std::pair<std::string, Texture*>(guid, texture));
	allTextures_sortedByPath.insert(std::pair<std::string, Texture*>(path, texture));
	std::cout << "TextureRepo::LoadTexture. Path - " << path << " GUID - " << guid << std::endl;
	return;
}

bool TextureRepo::TryGetTextureByGUID(const std::string& guid, Texture*& texture) {
	std::unordered_map<std::string, Texture*>::iterator it = allTextures_sortedByGUID.find(guid);
	if (it != allTextures_sortedByGUID.end()) {
		texture = it->second;
		return true;
	}

	std::cout << "  ################ GetTextureByGUID: " << guid << " not found" << std::endl;
	return false;
}