#pragma once

#include <unordered_map>
#include <string>
#include "Material.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Mat.h"

#include <string>
#include "ObjMetadata.h"

using namespace std;

class Database {

public:
	static void ImportAssets();
	static void ImportAssets(const string& path);
	static void ImportModel(const string& path);
	static void ImportModel_Node(aiNode* aiNode, const aiScene* aiScene, const string& dir, ObjMetadata& objMeta);
	static void ImportModel_Node_Mesh(aiMesh* aMesh, const aiScene* aiScene, const string& dir, ObjMetadata& objMeta);
	static void ImportModel_Node_Mesh_Texture(aiMaterial* aMat, aiTextureType aTextureType, const string& dir,Mat& matMeta);

	static void ClearInvalidMeta();
	static void ClearInvalidMeta(const string& path);

	static bool GenerateGUIDFromPath(const string& assetPath, string& guid);
	static string GetGUIDFromAssetPath(const string& assetPath);
	static string GetAssetPathFromGUID(const string& guid);

	static void LoadMaterialByGUID(const string& guid, Material& mat);
	static void LoadMaterialByAssetPath(const string& path, Material& mat);

private:
	static std::unordered_map<string, string> m_assetPath2GUID;
	static std::unordered_map<string, string> m_guid2AssetPath;


};

