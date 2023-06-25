#include "Database.h"

#include <iostream>
#include <vector>

#include <sstream>
#include <random>
#include <chrono>

std::unordered_map<std::string, std::string> Database::m_assetPath2GUID;
std::unordered_map<std::string, std::string> Database::m_guid2AssetPath;
// ------------------------------- Temp Asset
Material* Database::defaultMaterial;
Material* Database::defaultLightMaterial;
Material* Database::lightCubeMaterial;
Material* Database::depthMapMaterial;

void Database::LoadDatabase() {
	std::cout << "######################################### LoadDatabase ######################################### " << std::endl;
	m_assetPath2GUID = std::unordered_map<std::string, std::string>();
	m_guid2AssetPath = std::unordered_map<std::string, std::string>();

	LoadMaterials();
	LoadAssetTextures();
	LoadAssetShaders();
	LoadAssetModels();
}

void Database::LoadMaterials() {
	defaultMaterial = new Material();
	defaultMaterial->SetDiffuseTexture(1000);
	defaultMaterial->SetShader(1000);

	defaultLightMaterial = new Material();
	defaultLightMaterial->SetDiffuseTexture(1000);
	defaultLightMaterial->SetShader(2000);

	lightCubeMaterial = new Material();
	lightCubeMaterial->SetDiffuseTexture(2000);
	lightCubeMaterial->SetShader(1000);

	depthMapMaterial = new Material();
	depthMapMaterial->SetDiffuseTexture(1000);
	depthMapMaterial->SetShader(3000);
}

void Database::LoadAssetModels() {
	std::string modelPath1 = "asset/model/nanosuit/nanosuit.obj";

	std::string guid1 = GenerateGUIDFromPath(modelPath1);

	m_assetPath2GUID.insert(std::pair<std::string, std::string>(modelPath1, guid1));

	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid1, modelPath1));
}

void Database::LoadAssetTextures() {
	std::string texturePath1 = "asset/texture/jerry.png";
	std::string texturePath2 = "asset/texture/room.png";

	std::string guid1 = GenerateGUIDFromPath(texturePath1);
	std::string guid2 = GenerateGUIDFromPath(texturePath2);

	m_assetPath2GUID.insert(std::pair<std::string, std::string>(texturePath1, guid1));
	m_assetPath2GUID.insert(std::pair<std::string, std::string>(texturePath2, guid2));

	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid1, texturePath1));
	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid2, texturePath2));
}

void Database::LoadAssetShaders() {
	std::string shaderPath1 = "asset/shader/Default.shader";
	std::string shaderPath2 = "asset/shader/DefaultLight.shader";
	std::string shaderPath3 = "asset/shader/DepthMap.shader";

	std::string guid1 = GenerateGUIDFromPath(shaderPath1);
	std::string guid2 = GenerateGUIDFromPath(shaderPath2);
	std::string guid3 = GenerateGUIDFromPath(shaderPath3);

	m_assetPath2GUID.insert(std::pair<std::string, std::string>(shaderPath1, guid1));
	m_assetPath2GUID.insert(std::pair<std::string, std::string>(shaderPath2, guid2));
	m_assetPath2GUID.insert(std::pair<std::string, std::string>(shaderPath3, guid3));

	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid1, shaderPath1));
	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid2, shaderPath2));
	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid3, shaderPath3));
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

	std::string guid = ss.str();
	std::cout << "GenerateGUIDFromPath: Path - " << path << " GUID - " << guid << std::endl;
	return guid;
}

std::string Database::GetAssetPathFromGUID(const std::string& guid) {
	std::unordered_map<std::string, std::string>::iterator it = m_guid2AssetPath.find(guid);
	if (it != m_guid2AssetPath.end()) {
		return it->second;
	}

	std::cout << "################ GetAssetPathFromGUID: " << guid << " not found" << std::endl;
	return "";
}

std::string Database::GetGUIDFromAssetPath(const std::string& path) {
	std::unordered_map<std::string, std::string>::iterator it = m_assetPath2GUID.find(path);
	if (it != m_assetPath2GUID.end()) {
		return it->second;
	}

	std::cout << "################ GetGUIDFromAssetPath: " << path << " not found" << std::endl;
	return "";
}



