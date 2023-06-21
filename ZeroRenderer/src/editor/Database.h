#pragma once

#include <unordered_map>
#include <string>

class Database {

public:
	static std::unordered_map<std::string, std::string>* m_assetPath2GUID;
	static std::unordered_map<std::string, std::string>* m_guid2AssetPath;
	static void LoadDatabase();
	static void LoadAssetModels();
	static void LoadAssetTextures();
	static void LoadAssetShaders();
	static std::string GenerateGUIDFromPath(const std::string& assetPath);

};

