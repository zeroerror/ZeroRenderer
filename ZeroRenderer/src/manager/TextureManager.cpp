#include "TextureManager.h"

TextureManager::TextureManager() {
    allTextures = std::unordered_map<unsigned int, Texture*>();
}

TextureManager::~TextureManager() {}

unsigned int TextureManager::LoadTexture(const char* path) {
	Texture* texture = new Texture(path);
	unsigned int textureID = texture->GetID();
	allTextures.insert(std::pair<unsigned int, Texture*>(textureID, texture));
	return textureID;
}

void TextureManager::UseTexture(unsigned int textureID) {
	allTextures[textureID]->Bind();
}

void TextureManager::DeleteTexture(unsigned int textureID) {
	allTextures[textureID]->~Texture();
}

void TextureManager::DeleteAllTexture() {
	for (int i = 0; i < allTextures.size(); i++) {
		allTextures[i]->~Texture();
	}
}