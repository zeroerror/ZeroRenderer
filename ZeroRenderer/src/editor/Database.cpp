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
#include "ShaderMetadata.h"

using namespace std;
namespace fs = filesystem;

std::unordered_map<string, string> Database::m_assetPath2GUID;
std::unordered_map<string, string> Database::m_guid2AssetPath;

void Database::ImportAssets() {
	std::cout << "  +++++++++++++++++++++++++++++++++++++++ Database ImportAssets Start +++++++++++++++++++++++++++++++++++++++  " << std::endl;
	m_assetPath2GUID = std::unordered_map<string, string>();
	m_guid2AssetPath = std::unordered_map<string, string>();
	ImportAssets("asset");
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
				string guid;
				GenerateGUIDFromAssetPath(assetPath, guid);
				string metaPath = assetPath + FileSuffix::SUFFIX_META;
				if (!FileHelper::FileExist(metaPath)) {
					TextureMetadata texMeta = TextureMetadata();
					texMeta.guid = guid;
					texMeta.SerializeTo(metaPath);
				}
				else {
					TextureMetadata texMeta = TextureMetadata();
					texMeta.DeserializeFrom(metaPath);
					guid = texMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ Database: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_SHADER) {
				string guid;
				GenerateGUIDFromAssetPath(assetPath, guid);
				string metaPath = assetPath + FileSuffix::SUFFIX_META;
				if (!FileHelper::FileExist(metaPath)) {
					ShaderMetadata shaderMeta = ShaderMetadata();
					shaderMeta.guid = guid;
					shaderMeta.useLightingMVP = false;
					shaderMeta.SerializeTo(metaPath);
				}
				else {
					ShaderMetadata shaderMeta = ShaderMetadata();
					shaderMeta.DeserializeFrom(metaPath);
					guid = shaderMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ Database: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_MAT) {
				string guid;
				GenerateGUIDFromAssetPath(assetPath, guid);
				string metaPath = assetPath + FileSuffix::SUFFIX_META;
				if (!FileHelper::FileExist(metaPath)) {
					MaterialMetadata matMeta = MaterialMetadata();
					matMeta.guid = guid;
					matMeta.SerializeTo(metaPath);
				}
				else {
					MaterialMetadata matMeta = MaterialMetadata();
					matMeta.DeserializeFrom(metaPath);
					guid = matMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ Database: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_OBJ) {
				Database::ImportModel(assetPath);
			}
		}
	}
}

void Database::ImportModel(string& assetPath) {
	string guid;
	GenerateGUIDFromAssetPath(assetPath, guid);
	InsertToMap_AssetPath2GUID(assetPath, guid);
	InsertToMap_GUID2AssetPath(guid, assetPath);

	string objMetaPath = assetPath + FileSuffix::SUFFIX_META;
	if (FileHelper::FileExist(objMetaPath)) {
		return;
	}

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

	objMeta.SerializeTo(objMetaPath);

	std::cout << " +++ Database: Import " << objMetaPath << " guid - " << guid << std::endl;
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
	string matMetaPath = matPath + FileSuffix::SUFFIX_META;
	GenerateGUIDFromAssetPath(matPath, matMeta.guid);
	matMeta.SerializeTo(matMetaPath);
	std::cout << "Database: Generate mat meta " << matMetaPath << std::endl;

	objMeta.meshNames.push_back(meshName);
	objMeta.materialGUIDs.push_back(matMeta.guid);

	InsertToMap_GUID2AssetPath(matMeta.guid, matPath);
	InsertToMap_AssetPath2GUID(matPath, matMeta.guid);
}

void Database::ImportModel_Node_Mesh_Texture(aiMaterial* aMat, aiTextureType aTextureType, const string& dir, Mat& mat) {
	unsigned int textureCount = aMat->GetTextureCount(aTextureType);
	for (unsigned int i = 0; i < textureCount; i++) {
		aiString str;
		aMat->GetTexture(aTextureType, i, &str);
		string texPath = dir + str.C_Str();
		string texGUID;
		if (!TryGetGUIDFromAssetPath(texPath, texGUID)) {
			GenerateGUIDFromAssetPath(texPath, texGUID);
		}
		if (aTextureType == aiTextureType_DIFFUSE) {
			mat.diffuseTextureGUID = texGUID;
		}

		InsertToMap_GUID2AssetPath(texGUID, texPath);
		InsertToMap_AssetPath2GUID(texPath, texGUID);
	}
}

bool Database::SetMat_DiffuseTextureGUID(const string& matPath, const string& texturePath, const TextureType& textureType) {
	if (!AssetPathExist(texturePath)) return false;
	if (!AssetPathExist(matPath)) return false;

	string texGUID;
	if (!TryGetGUIDFromAssetPath(texturePath, texGUID)) {
		return false;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(matPath);
	if (textureType == TextureType::Diffuse)mat.diffuseTextureGUID = texGUID;
	mat.SerializeTo(matPath);
	return true;
}

bool Database::SetMat_ShaderGUID(const string& matPath, const string& shaderPath) {
	if (!AssetPathExist(matPath)) return false;

	string shaderGUID;
	if (!TryGetGUIDFromAssetPath(shaderPath, shaderGUID)) {
		return false;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(matPath);
	mat.shaderGUID = shaderGUID;
	mat.SerializeTo(matPath);
	return true;
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

void Database::GenerateGUIDFromAssetPath(string& assetPath, string& guid) {
	FileHelper::NormalizePath(assetPath);
	std::unordered_map<string, string>::iterator it = m_assetPath2GUID.find(assetPath);
	if (it != m_assetPath2GUID.end()) {
		guid = it->second;
		return;
	}

	std::hash<string> hasher;
	stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(assetPath);
	ss << std::hex << pathHash;

	guid = ss.str();
}

bool Database::TryGetAssetPathFromGUID(const string& guid, string& assetPath) {
	std::unordered_map<string, string>::iterator it = m_guid2AssetPath.find(guid);
	if (it != m_guid2AssetPath.end()) {
		assetPath = it->second;
		return true;
	}

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

	return false;
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
