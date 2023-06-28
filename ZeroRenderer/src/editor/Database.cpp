#include "Database.h"

#include <vector>
#include <random>

#include <iostream>
#include <sstream>
#include <filesystem>
#include <assimp/postprocess.h>
#include "FileSuffix.h"
#include "TextureMetadata.h"
#include "FileHelper.h"

using namespace std;
namespace fs = filesystem;

std::unordered_map<string, string> Database::m_assetPath2GUID;
std::unordered_map<string, string> Database::m_guid2AssetPath;
// ------------------------------- Temp Asset
Material* Database::defaultMaterial;
Material* Database::defaultLightMaterial;
Material* Database::lightCubeMaterial;
Material* Database::depthMapMaterial;

void Database::LoadDatabase() {
	std::cout << "  ######################################### Database Load Start ######################################### " << std::endl;
	m_assetPath2GUID = std::unordered_map<string, string>();
	m_guid2AssetPath = std::unordered_map<string, string>();

	LoadDatabase("asset");
	LoadMaterials();	// TODO - Meta instead of runtime Material
	LoadAssetModels();
	std::cout << "  ######################################### Database Load Completed ######################################### " << std::endl;
}

void Database::LoadDatabase(const string& dir) {
	fs::directory_iterator dirIt = fs::directory_iterator(dir);
	for (const auto& entry : dirIt) {
		fs::path path = entry.path();
		string pathStr = path.string();
		if (entry.is_directory()) {
			std::cout << "Directory -------- " << pathStr << std::endl;
			LoadDatabase(pathStr);
		}
		else {
			string assetPath = path.string();
			string extensionStr = path.extension().string();
			if (extensionStr == FileSuffix::SUFFIX_PNG) {
				string metaPath = assetPath + ".meta";
				std::cout  << "Generate texture meta - " << metaPath << std::endl;

				string guid;
				if (!GenerateGUIDFromPath(assetPath, guid)) {
					return;
				}
				TextureMetadata texMeta = TextureMetadata();
				texMeta.guid = guid;
				texMeta.SerializeTo(metaPath);
				m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));
				m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));
			}
			else if (extensionStr == FileSuffix::SUFFIX_SHADER) {
				string metaPath = assetPath + ".meta";
				std::cout << "Generate shader meta - " << metaPath << std::endl;

				string guid;
				if (!GenerateGUIDFromPath(assetPath, guid)) {
					return;
				}
				TextureMetadata texMeta = TextureMetadata();
				texMeta.guid = guid;
				texMeta.SerializeTo(metaPath);
				m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));
				m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));
			}
		}
	}
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

void Database::ClearInvalidMeta() {
	ClearInvalidMeta("asset");
}

void Database::ClearInvalidMeta(const string& dir) {
	fs::directory_iterator dirIt = fs::directory_iterator(dir);
	for (const auto& entry : dirIt) {
		fs::path path = entry.path();
		string pathStr = path.string();
		if (entry.is_directory()) {
			ClearInvalidMeta(pathStr);
		}
		else {
			string extensionStr = path.extension().string();
			if (extensionStr == FileSuffix::SUFFIX_META) {
				FileHelper::DeleteFile(path.string());
			}
		}
	}
}

void Database::LoadAssetModels() {
	LoadAssetModel("asset/model/nanosuit/nanosuit.obj");
}

void Database::LoadAssetModel(const string& assetPath) {
	string guid;
	if (!GenerateGUIDFromPath(assetPath, guid)) {
		return;
	}

	m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));

	m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(assetPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "  *********************************** Database::LoadAssetModel " << importer.GetErrorString() << std::endl;
		return;
	}

	size_t pos = assetPath.find_last_of("/");
	if (pos != string::npos) {
		string dir = assetPath.substr(0, pos + 1);
		ProcessNode(scene->mRootNode, scene, dir);
	}
}

void Database::ProcessNode(aiNode* aNode, const aiScene* aScene, const string& dir) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, dir);
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ProcessNode(childNode, aScene, dir);
	}
}

void Database::ProcessMesh(aiMesh* aMesh, const aiScene* aScene, const  string& dir) {
	if (aMesh->mMaterialIndex < 0) {
		return;
	}

	aiMaterial* aiMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
	const string typeName = "texture_diffuse";
	ProcessMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, typeName, dir);
}

void Database::ProcessMaterialTextures(aiMaterial* aMat, aiTextureType aTextureType, const string& typeName, const string& dir) {
	unsigned int textureCount = aMat->GetTextureCount(aTextureType);
	for (unsigned int i = 0; i < textureCount; i++) {
		aiString str;
		aMat->GetTexture(aTextureType, i, &str);
		string texPath = dir + str.C_Str();
		// TODO - Save to meta data
	}
}

bool Database::GenerateGUIDFromPath(const string& assetPath, string& guid) {
	std::unordered_map<string, string>::iterator it = m_assetPath2GUID.find(assetPath);
	if (it != m_assetPath2GUID.end()) {
		return false;
	}

	std::hash<string> hasher;
	stringstream ss;

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
	std::cout << "GenerateGUIDFromPath: assetPath - " << assetPath << " guid - " << guid << std::endl;
	return true;
}

string Database::GetAssetPathFromGUID(const string& guid) {
	std::unordered_map<string, string>::iterator it = m_guid2AssetPath.find(guid);
	if (it != m_guid2AssetPath.end()) {
		return it->second;
	}

	std::cout << "  *********************************** GetAssetPathFromGUID: " << guid << " not found" << std::endl;
	return "";
}

string Database::GetGUIDFromAssetPath(const string& path) {
	std::unordered_map<string, string>::iterator it;
	for (it = m_assetPath2GUID.begin(); it != m_assetPath2GUID.end(); it++) {
		string assetPath = it->first;
		if(FileHelper::PathEquals(assetPath, path)) {
			return it->second;
		}
	}

	std::cout << "  *********************************** GetGUIDFromAssetPath: " << path << " not found" << std::endl;
	return "";
}



