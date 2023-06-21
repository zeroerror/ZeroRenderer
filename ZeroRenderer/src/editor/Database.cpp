#include "Database.h"

#include <iostream>
#include <vector>

#include <sstream>
#include <random>
#include <chrono>

std::unordered_map<std::string, std::string>* Database::m_assetPath2GUID;
std::unordered_map<std::string, std::string>* Database::m_guid2AssetPath;

void Database::LoadDatabase() {
    std::cout << "######################################### LoadDatabase ######################################### " << std::endl;    
    m_assetPath2GUID = new std::unordered_map<std::string, std::string>();
    m_guid2AssetPath = new std::unordered_map<std::string, std::string>();

    LoadAssetTextures();
    LoadAssetShaders();
    LoadAssetModels();
}

void Database::LoadAssetModels() {
	std::string modelPath1 = "res/model/nanosuit/nanosuit.obj";

	std::string guid1 = GenerateGUIDFromPath(modelPath1);

	m_assetPath2GUID->insert(std::pair<std::string, std::string>(modelPath1, guid1));

	m_guid2AssetPath->insert(std::pair<std::string, std::string>(guid1, modelPath1));
}

void Database::LoadAssetTextures() {
	std::string texturePath1 = "res/texture/jerry.png";
	std::string texturePath2 = "res/texture/room.png";

	std::string guid1 = GenerateGUIDFromPath(texturePath1);
	std::string guid2 = GenerateGUIDFromPath(texturePath2);

	m_assetPath2GUID->insert(std::pair<std::string, std::string>(texturePath1, guid1));
	m_assetPath2GUID->insert(std::pair<std::string, std::string>(texturePath2, guid2));

	m_guid2AssetPath->insert(std::pair<std::string, std::string>(guid1, texturePath1));
	m_guid2AssetPath->insert(std::pair<std::string, std::string>(guid2, texturePath2));
}

void Database::LoadAssetShaders() {
 	std::string shaderPath1 = "res/shaders/Default.shader";
	std::string shaderPath2 = "res/shaders/DefaultLight.shader";
	std::string shaderPath3 = "res/shaders/DepthMap.shader";

	std::string guid1 = GenerateGUIDFromPath(shaderPath1);
	std::string guid2 = GenerateGUIDFromPath(shaderPath2);
    std::string guid3 = GenerateGUIDFromPath(shaderPath3);

	m_assetPath2GUID->insert(std::pair<std::string, std::string>(shaderPath1, guid1));
	m_assetPath2GUID->insert(std::pair<std::string, std::string>(shaderPath2, guid2));
    m_assetPath2GUID->insert(std::pair<std::string, std::string>(shaderPath3, guid3));

	m_guid2AssetPath->insert(std::pair<std::string, std::string>(guid1, shaderPath1));
	m_guid2AssetPath->insert(std::pair<std::string, std::string>(guid2, shaderPath2)); 
    m_guid2AssetPath->insert(std::pair<std::string, std::string>(guid3, shaderPath3));  
}

std::string Database::GenerateGUIDFromPath(const std::string& path) {
	std::hash<std::string> hasher;
	std::stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(path);
	ss << std::hex << pathHash;

	// 生成时间戳
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto value = now_ms.time_since_epoch().count();
	ss << std::hex << value;

	// 生成随机数
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis(0, 0xFFFFFFFFFFFFFFFF);
	ss << std::hex << dis(gen);

    std::cout << "GUID: " << ss.str() << std::endl;    
	return ss.str();
}



