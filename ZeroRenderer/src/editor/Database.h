#pragma once

#include "Mat.h"
#include "Material.h"
#include "ObjMetadata.h"
#include "TextureSlotCollection.h"

#include <unordered_map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;

struct AssetTreeNode {
	string assetName;
	unordered_map<string, AssetTreeNode*> childNodes;
	bool isExpanded;
	bool isDir;

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
			string curName = path.substr(offset, pos - offset);
			if (childNodes.find(curName) == childNodes.end()) {
				node = nullptr;
				return false;
			}

			curNode = childNodes.at(curName);
		} while (true);

		return false;
	}

};

class Database {

public:
	static void ImportAssets();
	static void ImportAssets(const string& path);
	static void ImportModel(string& path);
	static void ImportModel_Node(aiNode* aiNode, const aiScene* aiScene, const string& dir, ObjMetadata& objMeta);
	static void ImportModel_Node_Mesh(aiMesh* aMesh, const aiScene* aiScene, const string& dir, ObjMetadata& objMeta);
	static void ImportModel_Node_Mesh_Texture(aiMaterial* aMat, aiTextureType aTextureType, const string& dir, Mat& matMeta);

	static bool SetMat_DiffuseTextureGUID(const string& matPath, const string& texturePath, const unsigned int& textureSlot);
	static bool SetMat_ShaderGUID(const string& matPath, const string& shaderPath);

	static void ClearInvalidMeta();
	static void ClearInvalidMeta(const string& path);

	static void GenerateGUIDFromAssetPath(string& assetPath, string& guid);
	static bool TryGetGUIDFromAssetPath(const string& assetPath, string& guid);
	static bool TryGetAssetPathFromGUID(const string& guid, string& assetPath);

	static bool GUIDExist(const string& guid);
	static bool AssetPathExist(const string& path);

	static vector<string> GetAllAssetPaths();
	static AssetTreeNode* GetRootAssetTreeNode();
	static void MoveFile(const string& fromPath, const string& toPath);

private:
	static std::unordered_map<string, string> m_assetPath2GUID;
	static std::unordered_map<string, string> m_guid2AssetPath;
	static void InsertToMap_AssetPath2GUID(string& assetPath, const string& guid);
	static void InsertToMap_GUID2AssetPath(const string& guid, string& assetPath);
	static inline void FillToAssetTreeNode(AssetTreeNode* node, const string& path);

};

