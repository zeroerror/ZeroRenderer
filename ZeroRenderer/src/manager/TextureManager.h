#pragma once
#include <Texture.h>
#include <unordered_map>

class TextureManager {

public:
	TextureManager();
	~TextureManager();

	unsigned int LoadTexture(const char* path);
	void UseTexture(unsigned int shader);
	void DeleteTexture(unsigned int shader);
	void DeleteAllTexture();

private:
	std::unordered_map<unsigned int, Texture*> allTextures;

};

