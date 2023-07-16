#pragma once

#include "ObjMeta.h"
#include "SceneMeta.h"
#include "PrefabMeta.h"
#include "MaterialMeta.h"
#include "SkinMeshRendererMeta.h"

#include <unordered_map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;

struct AssetTreeNode {
	string assetPath;
	string assetName;
	bool isExpanded;
	bool isDir;
	AssetTreeNode* fatherNode;
	unordered_map<string, AssetTreeNode*> childNodes;

	bool TryGetNodeByPath(const string& path, AssetTreeNode*& node) {
		AssetTreeNode* curNode = this;
		string curPath = curNode->assetName;
		size_t offset = 0;

		do {
			size_t pos = path.find('/', offset);
			if (pos == string::npos) {
				string curName = path.substr(offset);
				if (curName == curNode->assetName) {
					node = curNode;
					return true;
				}

				return false;
			}

			offset = pos + 1;
			pos = path.find('/', offset);
			string curName;
			if (pos == string::npos) {
				curName = path.substr(offset);
			}
			else {
				curName = path.substr(offset, pos - offset);
			}

			unordered_map<string, AssetTreeNode*> curChilds = curNode->childNodes;
			if (curChilds.find(curName) == curChilds.end()) {
				node = nullptr;
				return false;
			}

			curNode = curChilds.at(curName);
		} while (true);

		return false;
	}

};

class EditorDatabase {

public:
	static void ImportAssets();
	static void ImportAssets(const string& path);

	static void ImportModel(string& path);
	static void ImportModel_Node(aiNode* aiNode, const aiScene* aiScene, const string& dir, ObjMeta& objMeta, PrefabMeta& prefabMeta);
	static void ImportModel_Node_Mesh(aiMesh* aMesh, const aiScene* aiScene, const unsigned int& meshIndex, const string& dir, ObjMeta& objMeta, PrefabMeta& prefabMeta);
	static void ImportModel_Node_Mesh_Texture(aiMaterial* aMat, aiTextureType aTextureType, const string& dir, MaterialMeta& matMeta);

	static void ClearFile(const unsigned int& suffixFlag);
	static void ClearFile(const string& path, const unsigned int& suffixFlag);

	static bool SetMat_DiffuseTextureGUID(const string& matPath, const string& texturePath, const unsigned int& textureSlot);
	static bool SetMat_ShaderGUID(const string& matPath, const string& shaderPath);

	static string GenerateGUIDFromAssetPath(const string& assetPath);
	static bool TryGetGUIDFromAssetPath(const string& assetPath, string& guid);
	static bool TryGetAssetPathFromGUID(const string& guid, string& assetPath);

	static bool GUIDExist(const string& guid);
	static bool AssetPathExist(const string& path);

	static vector<string> GetAllAssetPaths();
	static AssetTreeNode* GetRootAssetTreeNode();
	static void MoveFile(const string& fromPath, const string& toPath);

	static void GenerateDefaultSceneMeta();

private:
	static std::unordered_map<string, string> m_assetPath2GUID;
	static std::unordered_map<string, string> m_guid2AssetPath;
	static void InsertToMap_AssetPath2GUID(string& assetPath, const string& guid);
	static void InsertToMap_GUID2AssetPath(const string& guid, string& assetPath);
	static inline void FillToAssetTreeNode(AssetTreeNode* node, const string& path, size_t offset);

};

