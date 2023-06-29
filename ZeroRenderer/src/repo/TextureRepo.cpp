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

bool TextureRepo::TryLoadTextureByPath(const string& path, Texture*& texture) {
	string guid;
	if (!Database::TryGetGUIDFromAssetPath(path, guid)) {
		std::cout << "############## TextureRepo::LoadTextureByPath -  path doesn't exist : " << path << std::endl;
		return false;
	}

	if (_TryGetTextureByGUID(guid, texture)) {
		return true;
	}

	texture = new Texture(path);
	allTextures_sortedByGUID.insert(std::pair<std::string, Texture*>(guid, texture));
	allTextures_sortedByPath.insert(std::pair<std::string, Texture*>(path, texture));
	return true;
}

bool TextureRepo::TryLoadTextureByGUID(const string& guid, Texture*& texture) {
	string path;
	if (!Database::TryGetAssetPathFromGUID(guid, path)) {
		std::cout << "############## TextureRepo::LoadTextureByGUID -  guid doesn't exist : " << guid << std::endl;
		return false;
	}

	if (_TryGetTextureByGUID(guid, texture)) {
		return true;
	}

	texture = new Texture(path);
	allTextures_sortedByGUID.insert(std::pair<std::string, Texture*>(guid, texture));
	allTextures_sortedByPath.insert(std::pair<std::string, Texture*>(path, texture));
	return true;
}

bool TextureRepo::_TryGetTextureByGUID(const std::string& guid, Texture*& texture) {
	std::unordered_map<std::string, Texture*>::iterator it = allTextures_sortedByGUID.find(guid);
	if (it != allTextures_sortedByGUID.end()) {
		texture = it->second;
		return true;
	}

	std::cout << "  ################ GetTextureByGUID: " << guid << " not found" << std::endl;
	return false;
}