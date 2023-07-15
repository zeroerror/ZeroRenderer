#include "EditorDatabase.h"

#include <vector>
#include <random>

#include <iostream>
#include <sstream>
#include <filesystem>
#include "FileSuffix.h"
#include "FileHelper.h"
#include <assimp/postprocess.h>

#include "TextureMeta.h"
#include "ShaderMeta.h"
#include "TextureSlotCollection.h"
#include "Serialization.h"
#include "Serialization.h"
#include "EditorModelManager.h"

using namespace Serialization;
using namespace std;
namespace fs = filesystem;

std::unordered_map<string, string> EditorDatabase::m_assetPath2GUID;
std::unordered_map<string, string> EditorDatabase::m_guid2AssetPath;

void EditorDatabase::ImportAssets() {
	std::cout << "  +++++++++++++++++++++++++++++++++++++++ EditorDatabase ImportAssets Start +++++++++++++++++++++++++++++++++++++++  " << std::endl;
	m_assetPath2GUID = std::unordered_map<string, string>();
	m_guid2AssetPath = std::unordered_map<string, string>();
	ImportAssets("asset");
	std::cout << "  +++++++++++++++++++++++++++++++++++++++ EditorDatabase ImportAssets Completed +++++++++++++++++++++++++++++++++++++++ \n" << std::endl;
}

void EditorDatabase::ImportAssets(const string& dir) {
	fs::directory_iterator dirIt = fs::directory_iterator(dir);
	for (const auto& entry : dirIt) {
		if (entry.is_directory()) {
			fs::path path = entry.path();
			string pathStr = path.string();
			std::cout << "Directory ------------------- " << pathStr << std::endl;
			ImportAssets(pathStr);
		}
		else {
			fs::path path = entry.path();
			string assetPath = path.string();
			string extensionStr = path.extension().string();
			if (extensionStr == FileSuffix::SUFFIX_PNG) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(assetPath)) {
					TextureMeta texMeta = TextureMeta();
					texMeta.guid = guid;
					TextureMeta_SerializeTo(texMeta, assetPath);
				}
				else {
					TextureMeta texMeta = TextureMeta();
					TextureMeta_DeserializeFrom(&texMeta, assetPath);
					guid = texMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_SHADER) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(assetPath)) {
					ShaderMeta shaderMeta = ShaderMeta();
					shaderMeta.guid = guid;
					shaderMeta.useLightingMVP = false;
					ShaderMeta_SerializeTo(&shaderMeta, assetPath);
				}
				else {
					ShaderMeta shaderMeta = ShaderMeta();
					ShaderMeta_DeserializeFrom(&shaderMeta, assetPath);
					guid = shaderMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_MAT) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(assetPath)) {
					MaterialMeta materialMeta = MaterialMeta();
					materialMeta.guid = guid;
					MaterialMeta_SerializeTo(materialMeta, assetPath);
				}
				else {
					MaterialMeta materialMeta = MaterialMeta();
					MaterialMeta_DeserializeFrom(&materialMeta, assetPath);
					guid = materialMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << " guid - " << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_OBJ) {
				EditorDatabase::ImportModel(assetPath);
			}
			else if (extensionStr == FileSuffix::SUFFIX_SCENE) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(assetPath)) {
					SceneMeta sceneMeta = SceneMeta();
					sceneMeta.guid = guid;
					SceneMeta_SerializeTo(sceneMeta, assetPath);
				}
				else {
					SceneMeta sceneMeta = SceneMeta();
					SceneMeta_DeserializeFrom(&sceneMeta, assetPath);
					guid = sceneMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << " guid - " << guid << std::endl;
			}
		}
	}
}

void EditorDatabase::ImportModel(string& assetPath) {
	string guid = GenerateGUIDFromAssetPath(assetPath);
	InsertToMap_AssetPath2GUID(assetPath, guid);
	InsertToMap_GUID2AssetPath(guid, assetPath);

	string objMetaPath = assetPath + FileSuffix::SUFFIX_META;
	if (FileHelper::FileExist(objMetaPath)) {
		return;
	}

	// Obj meta
	ObjMeta objMeta = ObjMeta();
	objMeta.guid = guid;

	// Create Prefab Meta
	PrefabMeta prefabMeta = PrefabMeta();
	prefabMeta.AddComponentMeta<TransformMeta>();
	prefabMeta.AddComponentMeta<SkinMeshRendererMeta>();

	const aiScene *aScene = nullptr;
	if(!EditorModelManager::TryLoadModel(assetPath, aScene)){
		return;
	}

	size_t pos = assetPath.find_last_of("/");
	if (pos == string::npos) pos = assetPath.find_last_of("\\");
	if (pos != string::npos) {
		string dir = assetPath.substr(0, pos + 1);
		ImportModel_Node(aScene->mRootNode, aScene, dir, objMeta, prefabMeta);
	}

	ObjMeta_SerializeTo(objMeta, assetPath);

	pos = assetPath.find_last_of(".");
	string assetPathWithoutSuffix = assetPath.substr(0, pos);
	PrefabMeta_SerializeTo(prefabMeta, assetPathWithoutSuffix);
}

void EditorDatabase::ImportModel_Node(aiNode* aNode, const aiScene* aScene, const string& dir, ObjMeta& objMeta, PrefabMeta& prefabMeta) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		unsigned int meshIndex = aNode->mMeshes[i];
		aiMesh* mesh = aScene->mMeshes[meshIndex];
		ImportModel_Node_Mesh(mesh, aScene, meshIndex, dir, objMeta, prefabMeta);
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ImportModel_Node(childNode, aScene, dir, objMeta, prefabMeta);
	}
}

void EditorDatabase::ImportModel_Node_Mesh(aiMesh* aMesh, const aiScene* aScene, const unsigned int& meshIndex, const string& dir, ObjMeta& objMeta, PrefabMeta& prefabMeta) {
	SkinMeshRendererMeta* skinMeshRendererMeta = prefabMeta.GetComponentMeta<SkinMeshRendererMeta>();

	// MeshFilterMeta
	MeshFilterMeta* meshFilterMeta = new MeshFilterMeta();
	string meshPath = dir + aMesh->mName.C_Str();
	meshFilterMeta->modelGUID = objMeta.guid;
	meshFilterMeta->meshIndex = meshIndex;
	skinMeshRendererMeta->meshFilterMetas.push_back(meshFilterMeta);

	// MeshFilterMeta
	MeshRendererMeta* meshRendererMeta = new MeshRendererMeta();
	skinMeshRendererMeta->meshRendererMetas.push_back(meshRendererMeta);
	meshRendererMeta->materialGUID = "";

	if (aMesh->mMaterialIndex < 0) return;

	aiMaterial* aiMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
	string meshName = aMesh->mName.C_Str();

	MaterialMeta materialMeta = MaterialMeta();
	ImportModel_Node_Mesh_Texture(aiMaterial, aiTextureType_DIFFUSE, dir, materialMeta);
	ImportModel_Node_Mesh_Texture(aiMaterial, aiTextureType_SPECULAR, dir, materialMeta);
	string matPath = dir + meshName;
	string materialGUID = GenerateGUIDFromAssetPath(matPath);
	materialMeta.guid = materialGUID;
	Serialization::MaterialMeta_SerializeTo(materialMeta, matPath);

	meshRendererMeta->materialGUID = materialGUID;

	InsertToMap_GUID2AssetPath(materialGUID, matPath);
	InsertToMap_AssetPath2GUID(matPath, materialGUID);
}

void EditorDatabase::ImportModel_Node_Mesh_Texture(aiMaterial* aMat, aiTextureType aTextureType, const string& dir, MaterialMeta& materialMeta) {
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
			materialMeta.diffuseTextureGUID = texGUID;
		}
		else if (aTextureType == aiTextureType_SPECULAR) {
			materialMeta.specularTextureGUID = texGUID;
		}

		InsertToMap_GUID2AssetPath(texGUID, texPath);
		InsertToMap_AssetPath2GUID(texPath, texGUID);
	}
}

PrefabMeta EditorDatabase::CreateModelPrefab(const string& path) {
	PrefabMeta prefabMeta = PrefabMeta();
	/////////////////////////////////////////////////////////// todo
	return prefabMeta;
}

PrefabMeta EditorDatabase::CreateGameobjectPrefab(const GameObject& gameObject, const string& path) {
	PrefabMeta prefabMeta = PrefabMeta();
	/////////////////////////////////////////////////////////// todo
	return prefabMeta;
}

void EditorDatabase::ClearMetaFile() {
	ClearMetaFile("asset");
}

void EditorDatabase::ClearMetaFile(const string& dir) {
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

bool EditorDatabase::SetMat_DiffuseTextureGUID(const string& matPath, const string& texturePath, const unsigned int& textureSlot) {
	if (!AssetPathExist(texturePath)) return false;
	if (!AssetPathExist(matPath)) return false;

	string texGUID;
	if (!TryGetGUIDFromAssetPath(texturePath, texGUID)) {
		return false;
	}

	MaterialMeta materialMeta = MaterialMeta();
	MaterialMeta_DeserializeFrom(&materialMeta, matPath);
	if (textureSlot == TEX_SLOT_DIFFUSE_MAP)materialMeta.diffuseTextureGUID = texGUID;
	MaterialMeta_SerializeTo(materialMeta, matPath);
	return true;
}

bool EditorDatabase::SetMat_ShaderGUID(const string& matPath, const string& shaderPath) {
	if (!AssetPathExist(matPath)) return false;

	string shaderGUID;
	if (!TryGetGUIDFromAssetPath(shaderPath, shaderGUID)) {
		return false;
	}

	MaterialMeta mat = MaterialMeta();
	MaterialMeta_DeserializeFrom(&mat, matPath);
	mat.shaderGUID = shaderGUID;
	MaterialMeta_SerializeTo(mat, matPath);
	return true;
}

string EditorDatabase::GenerateGUIDFromAssetPath(string& assetPath) {
	FileHelper::NormalizePath(assetPath);
	std::hash<string> hasher;
	stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(assetPath);
	ss << std::hex << pathHash;

	return ss.str();
}

bool EditorDatabase::TryGetAssetPathFromGUID(const string& guid, string& assetPath) {
	std::unordered_map<string, string>::iterator it = m_guid2AssetPath.find(guid);
	if (it != m_guid2AssetPath.end()) {
		assetPath = it->second;
		return true;
	}

	return false;
}

bool EditorDatabase::TryGetGUIDFromAssetPath(const string& path, string& guid) {
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

bool EditorDatabase::GUIDExist(const string& guid) {
	unordered_map<string, string>::iterator it = m_guid2AssetPath.find(guid);
	return it != m_guid2AssetPath.end();
}

bool EditorDatabase::AssetPathExist(const string& path) {
	unordered_map<string, string>::iterator it = m_assetPath2GUID.find(path);
	return it != m_assetPath2GUID.end();
}

vector<string> EditorDatabase::GetAllAssetPaths() {
	unordered_map<string, string>::iterator it = m_assetPath2GUID.begin();
	vector<string> fileResources;
	for (; it != m_assetPath2GUID.end(); it++) {
		string assetPath = it->first;
		fileResources.push_back(assetPath);
	}
	return fileResources;
}

AssetTreeNode* EditorDatabase::GetRootAssetTreeNode() {
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

void EditorDatabase::FillToAssetTreeNode(AssetTreeNode* node, const string& path, size_t offset) {
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


void EditorDatabase::MoveFile(const string& fromPath, const string& toPath) {
	string fromGUID;
	if (!TryGetGUIDFromAssetPath(fromPath, fromGUID)) return;
	string toGUID;
	if (!TryGetGUIDFromAssetPath(toPath, toGUID))return;
}

void EditorDatabase::InsertToMap_AssetPath2GUID(string& assetPath, const string& guid) {
	FileHelper::NormalizePath(assetPath);
	m_assetPath2GUID.insert(std::pair<string, string>(assetPath, guid));
}

void EditorDatabase::InsertToMap_GUID2AssetPath(const string& guid, string& assetPath) {
	FileHelper::NormalizePath(assetPath);
	m_guid2AssetPath.insert(std::pair<string, string>(guid, assetPath));
}
