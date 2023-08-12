#include "TextureRepo.h"
#include "EditorDatabase.h"

TextureRepo::TextureRepo() {
	allTextures_sortedByPath = std::unordered_map<std::string, Texture*>();
	allTextures_sortedByGUID = std::unordered_map<std::string, Texture*>();
	_CreateDefaultDiffuseTexture();
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

bool TextureRepo::TryAddTexture(const std::string& guid, Texture*& texture) {
	if(!EditorDatabase::GUIDExist(guid)) {
		return false;
	}

	allTextures_sortedByGUID.insert(std::pair<std::string, Texture*>(guid, texture));
	
	string path;
	if (EditorDatabase::TryGetAssetPathFromGUID(guid, path)) {
		allTextures_sortedByPath.insert(std::pair<std::string, Texture*>(path, texture));
	}

	return true;
}

bool TextureRepo::TryGetTextureByGUID(const std::string& guid, Texture*& texture) {
	std::unordered_map<std::string, Texture*>::iterator it = allTextures_sortedByGUID.find(guid);
	if (it != allTextures_sortedByGUID.end()) {
		texture = it->second;
		return true;
	}
	return false;
}

Texture* TextureRepo::DefaultDiffuseTexture() { return this->_defaultDiffuseTexture; }
void TextureRepo::_CreateDefaultDiffuseTexture(){
	this->_defaultDiffuseTexture = new Texture("asset/texture/default.png");
}
