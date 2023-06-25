#include "Database.h"

#include <iostream>
#include <vector>

#include <sstream>
#include <random>
#include <chrono>
#include <assimp/postprocess.h>

std::unordered_map<std::string, std::string> Database::m_assetPath2GUID;
std::unordered_map<std::string, std::string> Database::m_guid2AssetPath;
// ------------------------------- Temp Asset
Material* Database::defaultMaterial;
Material* Database::defaultLightMaterial;
Material* Database::lightCubeMaterial;
Material* Database::depthMapMaterial;

void Database::LoadDatabase() {
	std::cout << "  ######################################### Database Load Start ######################################### " << std::endl;
	m_assetPath2GUID = std::unordered_map<std::string, std::string>();
	m_guid2AssetPath = std::unordered_map<std::string, std::string>();

	LoadAssetTextures();
	LoadAssetShaders();
	LoadMaterials();	// TODO - Meta instead of runtime Material
	LoadAssetModels();
	std::cout << "  ######################################### Database Load Completed ######################################### " << std::endl;
}

void Database::LoadMaterials() {
	defaultMaterial = new Material();
	defaultMaterial->diffuseTextureGUID = GetGUIDFromAssetPath("asset/texture/default.png");
	defaultMaterial->shaderGUID = GetGUIDFromAssetPath("asset/shader/Default.shader");

	defaultLightMaterial = new Material();
	defaultLightMaterial->diffuseTextureGUID = GetGUIDFromAssetPath("asset/texture/default.png");
	defaultLightMaterial->shaderGUID = GetGUIDFromAssetPath("asset/shader/DefaultLight.shader");

	lightCubeMaterial = new Material();
	lightCubeMaterial->diffuseTextureGUID = GetGUIDFromAssetPath("asset/texture/jerry.png");
	lightCubeMaterial->shaderGUID = GetGUIDFromAssetPath("asset/shader/Default.shader");

	depthMapMaterial = new Material();
	depthMapMaterial->diffuseTextureGUID = GetGUIDFromAssetPath("asset/texture/default.png");
	depthMapMaterial->shaderGUID = GetGUIDFromAssetPath("asset/shader/DepthMap.shader");
}

void Database::LoadAssetModels() {
	LoadAssetModel("asset/model/nanosuit/nanosuit.obj");
}

void Database::LoadAssetModel(const std::string& assetPath) {
	std::string guid;
	if (!GenerateGUIDFromPath(assetPath, guid)) {
		return;
	}

	m_assetPath2GUID.insert(std::pair<std::string, std::string>(assetPath, guid));

	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid, assetPath));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(assetPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "  *********************************** Database::LoadAssetModel " << importer.GetErrorString() << std::endl;
		return;
	}

	size_t pos = assetPath.find_last_of("/");
	if (pos != std::string::npos) {
		std::string dir = assetPath.substr(0, pos + 1);
		ProcessNode(scene->mRootNode, scene, dir);
	}
}

void Database::ProcessNode(aiNode* aNode, const aiScene* aScene, const std::string& dir) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, dir);
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ProcessNode(childNode, aScene, dir);
	}
}

void Database::ProcessMesh(aiMesh* aMesh, const aiScene* aScene, const  std::string& dir) {
	if (aMesh->mMaterialIndex < 0) {
		return;
	}

	aiMaterial* aiMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
	const std::string typeName = "texture_diffuse";
	ProcessMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, typeName, dir);
}

void Database::ProcessMaterialTextures(aiMaterial* aMat, aiTextureType aTextureType, const std::string& typeName, const std::string& dir) {
	// TODO - Save to meta data
	unsigned int textureCount = aMat->GetTextureCount(aTextureType);
	for (unsigned int i = 0; i < textureCount; i++) {
		aiString str;
		aMat->GetTexture(aTextureType, i, &str);
		std::string texPath = dir + str.C_Str();
		Database::LoadAssetTexture(texPath);
	}
}

void Database::LoadAssetTextures() {
	LoadAssetTexture("asset/texture/default.png");
	LoadAssetTexture("asset/texture/jerry.png");
	LoadAssetTexture("asset/texture/room.png");
}

void Database::LoadAssetTexture(const std::string& assetPath) {
	std::string guid;
	if (!GenerateGUIDFromPath(assetPath, guid)) {
		return;
	}

	m_assetPath2GUID.insert(std::pair<std::string, std::string>(assetPath, guid));
	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid, assetPath));
	std::cout << "Database::LoadAssetTexture: path - " << assetPath << " guid - " << guid << std::endl;
}

void Database::LoadAssetShaders() {
	LoadAssetShader("asset/shader/Default.shader");
	LoadAssetShader("asset/shader/DefaultLight.shader");
	LoadAssetShader("asset/shader/DepthMap.shader");
}

void Database::LoadAssetShader(const std::string& assetPath) {
	std::string guid;
	if (!GenerateGUIDFromPath(assetPath, guid)) {
		return;
	}

	m_assetPath2GUID.insert(std::pair<std::string, std::string>(assetPath, guid));
	m_guid2AssetPath.insert(std::pair<std::string, std::string>(guid, assetPath));
	std::cout << "Database::LoadAssetShader: path - " << assetPath << " guid - " << guid << std::endl;
}

bool Database::GenerateGUIDFromPath(const std::string& assetPath, std::string& guid) {
	std::unordered_map<std::string, std::string>::iterator it = m_assetPath2GUID.find(assetPath);
	if (it != m_assetPath2GUID.end()) {
		return false;
	}

	std::hash<std::string> hasher;
	std::stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(assetPath);
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

	guid = ss.str();
	//std::cout << "GenerateGUIDFromPath: path - " << path << " guid - " << guid << std::endl;
	return true;
}

std::string Database::GetAssetPathFromGUID(const std::string& guid) {
	std::unordered_map<std::string, std::string>::iterator it = m_guid2AssetPath.find(guid);
	if (it != m_guid2AssetPath.end()) {
		return it->second;
	}

	std::cout << "  *********************************** GetAssetPathFromGUID: " << guid << " not found" << std::endl;
	return "";
}

std::string Database::GetGUIDFromAssetPath(const std::string& path) {
	std::unordered_map<std::string, std::string>::iterator it = m_assetPath2GUID.find(path);
	if (it != m_assetPath2GUID.end()) {
		return it->second;
	}

	std::cout << "  *********************************** GetGUIDFromAssetPath: " << path << " not found" << std::endl;
	return "";
}



