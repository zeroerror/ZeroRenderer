#pragma once
#include <string>
#include <Texture.h>
#include <unordered_map>

using namespace std;

class TextureRepo {

public:
	TextureRepo();
	~TextureRepo();

	bool TryAddTexture(const string& guid, Texture*& texture);
	bool TryGetTextureByGUID(const std::string& guid, Texture*& texture);

private:
	std::unordered_map<std::string, Texture*> allTextures_sortedByPath;
	std::unordered_map<std::string, Texture*> allTextures_sortedByGUID;

};

