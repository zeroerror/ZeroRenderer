#pragma once
#include <Texture.h>
#include <unordered_map>

class TextureRepo {

public:
	TextureRepo();
	~TextureRepo();

	unsigned int LoadTexture(const char* path);
	Texture* GetTexture(unsigned int textureID);

private:
	std::unordered_map<unsigned int, Texture*> allTextures;

};

