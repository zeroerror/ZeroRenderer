#pragma once
#include <Texture.h>
#include <unordered_map>

class TextureRepo {

public:
	TextureRepo();
	~TextureRepo();

	std::string LoadTexture(const char* path);
	Texture* GetTextureByGUID(const std::string& guid);

private:
	std::unordered_map<std::string, Texture*> allTextures;

};

