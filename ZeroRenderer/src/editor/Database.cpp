#include "Database.h"

#include <vector>
#include <random>

#include <iostream>
#include <sstream>
#include <filesystem>
#include "FileSuffix.h"
#include "FileHelper.h"
#include <assimp/postprocess.h>

#include "TextureMeta.h"
#include "MatMeta.h"
#include "ShaderMeta.h"
#include "TextureSlotCollection.h"
#include "Serialization.h"

using namespace Serialization;
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
				string guid = GenerateGUIDFromAssetPath(assetPath);
				string metaPath = assetPath + FileSuffix::SUFFIX_META;
				if (!FileHelper::FileExist(metaPath)) {
					TextureMeta texMeta = TextureMeta();
					texMeta.guid = guid;
					TextureMeta_SerializeTo(texMeta, metaPath);
				}
				else {
					TextureMeta texMeta = TextureMeta();
					TextureMeta_DeserializeFrom(&texMeta, metaPath);
					guid = texMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ Database: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_SHADER) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				string metaPath = assetPath + FileSuffix::SUFFIX_META;
				if (!FileHelper::FileExist(metaPath)) {
					ShaderMeta shaderMeta = ShaderMeta();
					shaderMeta.guid = guid;
					shaderMeta.useLightingMVP = false;
					ShaderMeta_SerializeTo(&shaderMeta, metaPath);
				}
				else {
					ShaderMeta shaderMeta = ShaderMeta();
					ShaderMeta_DeserializeFrom(&shaderMeta, metaPath);
					guid = shaderMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ Database: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_MAT) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				string metaPath = assetPath + FileSuffix::SUFFIX_META;
				if (!FileHelper::FileExist(metaPath)) {
					MatMeta matMeta = MatMeta();
					matMeta.guid = guid;
					MatMeta_SerializeTo(&matMeta, metaPath);
				}
				else {
					MatMeta matMeta = MatMeta();
					MatMeta_DeserializeFrom(&matMeta, metaPath);
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
	string guid = GenerateGUIDFromAssetPath(assetPath);
	InsertToMap_AssetPath2GUID(assetPath, guid);
	InsertToMap_GUID2AssetPath(guid, assetPath);

	string objMetaPath = assetPath + FileSuffix::SUFFIX_META;
	if (FileHelper::FileExist(objMetaPath)) {
		return;
	}

	ObjMeta objMeta = ObjMeta();
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

	ObjMeta_SerializeTo(objMeta, objMetaPath);
	std::cout << " +++ Database: Import " << objMetaPath << " guid - " << guid << std::endl;
}

void Database::ImportModel_Node(aiNode* aNode, const aiScene* aScene, const string& dir, ObjMeta& objMeta) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ImportModel_Node_Mesh(mesh, aScene, dir, objMeta);
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ImportModel_Node(childNode, aScene, dir, objMeta);
	}
}

void Database::ImportModel_Node_Mesh(aiMesh* aMesh, const aiScene* aScene, const  string& dir, ObjMeta& objMeta) {
	if (aMesh->mMaterialIndex < 0) {
		return;
	}

	aiMaterial* aiMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
	string meshName = aMesh->mName.C_Str();

	Mat mat = Mat();
	ImportModel_Node_Mesh_Texture(aiMaterial, aiTextureType_DIFFUSE, dir, mat);
	ImportModel_Node_Mesh_Texture(aiMaterial, aiTextureType_SPECULAR, dir, mat);
	string matPath = dir + meshName + ".mat";
	mat.SerializeTo(matPath);
	std::cout << "Database: Generate mat " << matPath << std::endl;

	MatMeta matMeta = MatMeta();
	string matMetaPath = matPath + FileSuffix::SUFFIX_META;
	matMeta.guid = GenerateGUIDFromAssetPath(matPath);
	Serialization::MatMeta_SerializeTo(&matMeta, matMetaPath);
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
			texGUID = GenerateGUIDFromAssetPath(texPath);
		}
		if (aTextureType == aiTextureType_DIFFUSE) {
			mat.diffuseTextureGUID = texGUID;
		}
		else if (aTextureType == aiTextureType_SPECULAR) {
			mat.specularTextureGUID = texGUID;
		}

		InsertToMap_GUID2AssetPath(texGUID, texPath);
		InsertToMap_AssetPath2GUID(texPath, texGUID);
	}
}

bool Database::SetMat_DiffuseTextureGUID(const string& matPath, const string& texturePath, const unsigned int& textureSlot) {
	if (!AssetPathExist(texturePath)) return false;
	if (!AssetPathExist(matPath)) return false;

	string texGUID;
	if (!TryGetGUIDFromAssetPath(texturePath, texGUID)) {
		return false;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(matPath);
	if (textureSlot == TEX_SLOT_DIFFUSE_MAP)mat.diffuseTextureGUID = texGUID;
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

void Database::ClearMetaFile() {
	ClearMetaFile("asset");
}

void Database::ClearMetaFile(const string& dir) {
	fs::directory_iterator dirIt = fs::directory_iterator(dir);
	for (const auto& entry : dirIt) {
		fs::path path = entry.path();
		string pathStr = path.string();
		if (entry.is_directory()) {
			ClearMetaFile(pathStr);
		}
		else {
			string extensionStr = path.extension().string();
			if (extensionStr == FileSuffix::SUFFIX_META) {
				FileHelper::DeleteFile(path.string());
			}
		}
	}
}

string Database::GenerateGUIDFromAssetPath(string& assetPath) {
	FileHelper::NormalizePath(assetPath);
	std::hash<string> hasher;
	stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(assetPath);
	ss << std::hex << pathHash;

	return ss.str();
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

vector<string> Database::GetAllAssetPaths() {
	unordered_map<string, string>::iterator it = m_assetPath2GUID.begin();
	vector<string> fileResources;
	for (; it != m_assetPath2GUID.end(); it++) {
		string assetPath = it->first;
		fileResources.push_back(assetPath);
	}
	return fileResources;
}

AssetTreeNode* Database::GetRootAssetTreeNode() {
	vector<string> allAssetPaths = GetAllAssetPaths();
	AssetTreeNode* rootNode = new AssetTreeNode();
	rootNode->assetPath = "asset";
	rootNode->assetName = "asset";
	rootNode->childNodes = unordered_map<string, AssetTreeNode*>();
	for (auto path : allAssetPaths) {
		FillToAssetTreeNode(rootNode, path, 0);
	}

	return rootNode;
}

void Database::FillToAssetTreeNode(AssetTreeNode* node, const string& path, size_t offset) {
	offset = path.find('/', offset);
	if (offset == string::npos) {
		return;
	}

	offset++;

	size_t nexPos = path.find('/', offset);
	string assetPath = path.substr(0, offset - 1);
	string assetName;
	if (nexPos == string::npos) {
		assetName = path.substr(offset);
		assetPath = path.substr(0);
	}
	else {
		size_t len = nexPos - offset;
		assetName = path.substr(offset, len);
		assetPath = path.substr(0, nexPos);
	}

	AssetTreeNode* childNode;
	if (node->childNodes.find(assetName) != node->childNodes.end()) {
		childNode = node->childNodes.at(assetName);
	}
	else {
		childNode = new AssetTreeNode();
		childNode->assetPath = assetPath;
		childNode->assetName = assetName;
		childNode->childNodes = unordered_map<string, AssetTreeNode*>();
		childNode->fatherNode = node;
		node->childNodes.insert(pair<string, AssetTreeNode*>(assetName, childNode));
	}

	node->isDir = true;
	FillToAssetTreeNode(childNode, path, offset);
}


void Database::MoveFile(const string& fromPath, const string& toPath) {
	string fromGUID;
	if (!TryGetGUIDFromAssetPath(fromPath, fromGUID)) return;
	string toGUID;
	if (!TryGetGUIDFromAssetPath(toPath, toGUID))return;
}

void Database::InsertToMap_AssetPath2GUID(string& assetPath, const string& guid) {
	FileHelper::NormalizePath(assetPath);
	m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));
}

void Database::InsertToMap_GUID2AssetPath(const string& guid, string& assetPath) {
	FileHelper::NormalizePath(assetPath);
	m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));
}
