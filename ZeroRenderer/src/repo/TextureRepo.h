#pragma once
#include <string>
#include <Texture.h>
#include <unordered_map>

using namespace std;

class TextureRepo {

public:
	TextureRepo();
	~TextureRepo();

	bool TryLoadTextureByPath(const string& path, Texture*& texture);
	bool TryLoadTextureByGUID(const string& guid, Texture*& texture);

private:
	std::unordered_map<std::string, Texture*> allTextures_sortedByPath;
	std::unordered_map<std::string, Texture*> allTextures_sortedByGUID;
	bool _TryGetTextureByGUID(const std::string& guid, Texture*& texture);

};

