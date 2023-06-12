#include "TextureRepo.h"

TextureRepo::TextureRepo() {
	allTextures = std::unordered_map<unsigned int, Texture*>();
}

TextureRepo::~TextureRepo() {
	std::cout << "TextureRepo::~TextureRepo() " << std::endl;
	for (auto& texture : allTextures) {
		texture.second->~Texture();
	}
}

Texture* TextureRepo::GetTexture(unsigned int textureID) {
	return allTextures[textureID];
}

unsigned int TextureRepo::LoadTexture(const char* path) {
	Texture* texture = new Texture(path);
	unsigned int textureID = texture->GetID();
	allTextures.insert(std::pair<unsigned int, Texture*>(textureID, texture));
	return textureID;
}