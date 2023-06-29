#include "Database.h"

#include <vector>
#include <random>

#include <iostream>
#include <sstream>
#include <filesystem>
#include "FileSuffix.h"
#include "FileHelper.h"
#include <assimp/postprocess.h>

#include "TextureMetadata.h"
#include "MaterialMetadata.h"

using namespace std;
namespace fs = filesystem;

std::unordered_map<string, string> Database::m_assetPath2GUID;
std::unordered_map<string, string> Database::m_guid2AssetPath;

void Database::ImportAssets() {
	std::cout << "  +++++++++++++++++++++++++++++++++++++++ Database ImportAssets Start +++++++++++++++++++++++++++++++++++++++  " << std::endl;
	m_assetPath2GUID = std::unordered_map<string, string>();
	m_guid2AssetPath = std::unordered_map<string, string>();
	ImportAssets("asset");
	//Mat mat = Mat();
	//mat.shaderGUID = TryGetGUIDFromAssetPath("asset/shader/Defalut.shader");
	//mat.diffuseTextureGUID = TryGetGUIDFromAssetPath("asset/texture/default.png");
	//mat.diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//mat.specularIntensity = 0.0f;
	//mat.shininess = 0.0f;
	//mat.SerializeTo("asset/material/default.mat");
	//mat = Mat();
	//mat.shaderGUID = TryGetGUIDFromAssetPath("asset/shader/DefaultLight.shader");
	//mat.diffuseTextureGUID = TryGetGUIDFromAssetPath("asset/texture/default.png");
	//mat.diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//mat.specularIntensity = 0.0f;
	//mat.shininess = 0.0f;
	//mat.SerializeTo("asset/material/defaultLight.mat");
	//mat = Mat();
	//mat.shaderGUID = TryGetGUIDFromAssetPath("asset/shader/DepthMap.shader");
	//mat.diffuseTextureGUID = TryGetGUIDFromAssetPath("asset/texture/default.png");
	//mat.diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//mat.specularIntensity = 0.0f;
	//mat.shininess = 0.0f;
	//mat.SerializeTo("asset/material/depthMap.mat");
	//mat = Mat();
	//mat.shaderGUID = TryGetGUIDFromAssetPath("asset/shader/DefaultLight.shader");
	//mat.diffuseTextureGUID = TryGetGUIDFromAssetPath("asset/texture/default.png");
	//mat.diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//mat.specularIntensity = 0.0f;
	//mat.shininess = 0.0f;
	//mat.SerializeTo("asset/material/lightCube.mat");
	std::cout << "  +++++++++++++++++++++++++++++++++++++++ Database ImportAssets Completed +++++++++++++++++++++++++++++++++++++++ \n" << std::endl;
}

void Database::ImportAssets(const string& dir) {
	fs::directory_iterator dirIt = fs::directory_iterator(dir);
	for (const auto& entry : dirIt) {
		fs::path path = entry.path();
		string pathStr = path.string();
		if (entry.is_directory()) {
			std::cout << "Directory ------------------- " << pathStr << std::endl;
			ImportAssets(pathStr);
		}
		else {
			string assetPath = path.string();
			string extensionStr = path.extension().string();
			if (extensionStr == FileSuffix::SUFFIX_PNG) {
				string metaPath = assetPath + FileSuffix::META;
				std::cout << "Database: Generate texture meta - " << metaPath << std::endl;

				string guid;
				if (!GenerateGUIDFromPath(assetPath, guid)) {
					return;
				}
				TextureMetadata texMeta = TextureMetadata();
				texMeta.guid = guid;
				texMeta.SerializeTo(metaPath);
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
			}
			else if (extensionStr == FileSuffix::SUFFIX_SHADER) {
				string metaPath = assetPath + FileSuffix::META;
				std::cout << "Database: Generate shader meta - " << metaPath << std::endl;

				string guid;
				GenerateGUIDFromPath(assetPath, guid);
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);

				TextureMetadata texMeta = TextureMetadata();
				texMeta.guid = guid;
				texMeta.SerializeTo(metaPath);
			}
			else if (extensionStr == FileSuffix::SUFFIX_MAT) {
				string metaPath = assetPath + FileSuffix::META;
				std::cout << "Database: Generate material meta - " << metaPath << std::endl;

				string guid;
				GenerateGUIDFromPath(assetPath, guid);
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);

				MaterialMetadata matMeta = MaterialMetadata();
				matMeta.guid = guid;
				matMeta.SerializeTo(metaPath);
			}
			else if (extensionStr == FileSuffix::SUFFIX_OBJ) {
				Database::ImportModel(assetPath);
			}
		}
	}
}

void Database::ImportModel(const string& assetPath) {
	string guid;
	GenerateGUIDFromPath(assetPath, guid);
	m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));
	m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));

	ObjMetadata objMeta = ObjMetadata();
	objMeta.guid = guid;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(assetPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "  *********************************** Database::LoadAssetModel " << importer.GetErrorString() << std::endl;
		return;
	}

	size_t pos = assetPath.find_last_of("/");
	if (pos == string::npos) pos = assetPath.find_last_of("\\");
	if (pos != string::npos) {
		string dir = assetPath.substr(0, pos + 1);
		ImportModel_Node(scene->mRootNode, scene, dir, objMeta);
	}

	string objMetaPath = assetPath + FileSuffix::META;
	objMeta.SerializeTo(objMetaPath);
	std::cout << "Database: Generate obj meta " << objMetaPath << std::endl;
}

void Database::ImportModel_Node(aiNode* aNode, const aiScene* aScene, const string& dir, ObjMetadata& objMeta) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ImportModel_Node_Mesh(mesh, aScene, dir, objMeta);
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ImportModel_Node(childNode, aScene, dir, objMeta);
	}
}

void Database::ImportModel_Node_Mesh(aiMesh* aMesh, const aiScene* aScene, const  string& dir, ObjMetadata& objMeta) {
	if (aMesh->mMaterialIndex < 0) {
		return;
	}

	aiMaterial* aiMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
	string meshName = aMesh->mName.C_Str();

	Mat mat = Mat();
	ImportModel_Node_Mesh_Texture(aiMaterial, aiTextureType_DIFFUSE, dir, mat);
	string matPath = dir + meshName + ".mat";
	mat.SerializeTo(matPath);
	std::cout << "Database: Generate mat " << matPath << std::endl;

	MaterialMetadata matMeta = MaterialMetadata();
	string matMetaPath = matPath + FileSuffix::META;
	GenerateGUIDFromPath(matPath, matMeta.guid);
	matMeta.SerializeTo(matMetaPath);
	std::cout << "Database: Generate mat meta " << matMetaPath << std::endl;

	objMeta.meshNames.push_back(meshName);
	objMeta.materialGUIDs.push_back(matMeta.guid);
}

void Database::ImportModel_Node_Mesh_Texture(aiMaterial* aMat, aiTextureType aTextureType, const string& dir, Mat& mat) {
	unsigned int textureCount = aMat->GetTextureCount(aTextureType);
	for (unsigned int i = 0; i < textureCount; i++) {
		aiString str;
		aMat->GetTexture(aTextureType, i, &str);
		string texPath = dir + str.C_Str();
		string texGUID;
		if (!TryGetGUIDFromAssetPath(texPath, texGUID)) {
			GenerateGUIDFromPath(texPath, texGUID);
		}
		if (aTextureType == aiTextureType_DIFFUSE) {
			mat.diffuseTextureGUID = texGUID;
		}
	}
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
			if (extensionStr == FileSuffix::META) {
				FileHelper::DeleteFile(path.string());
			}
		}
	}
}

bool Database::GenerateGUIDFromPath(const string& assetPath, string& guid) {
	std::unordered_map<string, string>::iterator it = m_assetPath2GUID.find(assetPath);
	if (it != m_assetPath2GUID.end()) {
		guid = it->second;
		return false;
	}

	std::hash<string> hasher;
	stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(assetPath);
	ss << std::hex << pathHash;

	// // 生成时间戳
	// auto now = std::chrono::system_clock::now();
	// auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	// auto value = now_ms.time_since_epoch().count();
	// ss << std::hex << value;

	// // 生成随机数
	// std::random_device rd;
	// std::mt19937 gen(rd());
	// std::uniform_int_distribution<unsigned long long> dis(0, 0xFFFFFFFFFFFFFFFF);
	// ss << std::hex << dis(gen);

	guid = ss.str();
	std::cout << "Database: GenerateGUIDFromPath - " << assetPath << " guid - " << guid << std::endl;
	return true;
}

bool Database::TryGetAssetPathFromGUID(const string& guid, string& assetPath) {
	std::unordered_map<string, string>::iterator it = m_guid2AssetPath.find(guid);
	if (it != m_guid2AssetPath.end()) {
		assetPath = it->second;
		return true;
	}

	std::cout << "Database: GetAssetPathFromGUID - " << guid << " not found" << std::endl;
	return false;
}

bool Database::TryGetGUIDFromAssetPath(const string& path, string& guid) {
	std::unordered_map<string, string>::iterator it;
	for (it = m_assetPath2GUID.begin(); it != m_assetPath2GUID.end(); it++) {
		string assetPath = it->first;
		if (FileHelper::PathEquals(assetPath, path)) {
			guid = it->second;
			return true;
		}
	}

	std::cout << "Database: TryGetGUIDFromAssetPath - " << path << " not found" << std::endl;
	return false;
}

bool Database::TryLoadMaterialByGUID(const string& guid, Material*& material) {
	if (!GUIDExist(guid)) {
		return false;
	}

	string matPath;
	if (!TryGetAssetPathFromGUID(guid, matPath)) {
		return false;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(matPath);
	material->shaderGUID = mat.shaderGUID;
	material->diffuseTextureGUID = mat.diffuseTextureGUID;
	material->diffuseColor = mat.diffuseColor;
	material->specularIntensity = mat.specularIntensity;
	material->shininess = mat.shininess;

	return true;
}

bool Database::TryLoadMaterialByAssetPath(const string& path, Material*& material) {
	if (!AssetPathExist(path)) {
		return false;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(path);
	material->shaderGUID = mat.shaderGUID;
	material->diffuseTextureGUID = mat.diffuseTextureGUID;
	material->diffuseColor = mat.diffuseColor;
	material->specularIntensity = mat.specularIntensity;
	material->shininess = mat.shininess;

	return true;
}

bool Database::GUIDExist(const string& guid) {
	unordered_map<string, string>::iterator it = m_guid2AssetPath.find(guid);
	return it != m_guid2AssetPath.end();
}

bool Database::AssetPathExist(const string& path) {
	unordered_map<string, string>::iterator it = m_assetPath2GUID.find(path);
	return it != m_assetPath2GUID.end();
}

void Database::InsertToMap_AssetPath2GUID(string& assetPath, const string& guid) {
	FileHelper::NormalizePath(assetPath);
	m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));
}

void Database::InsertToMap_GUID2AssetPath(const string& guid, string& assetPath) {
	FileHelper::NormalizePath(assetPath);
	m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));
}
