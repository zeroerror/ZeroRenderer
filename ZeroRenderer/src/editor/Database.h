#pragma once

#include <unordered_map>
#include <string>
#include "Material.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>

using namespace std;

class Database {

public:
	static Material* defaultMaterial;
	static Material* defaultLightMaterial;
	static Material* lightCubeMaterial;
	static Material* depthMapMaterial;

	static void LoadDatabase();
	static void LoadDatabase(const string& path);
	static void ClearInvalidMeta();
	static void ClearInvalidMeta(const string& path);
	static void LoadMaterials();
	static void LoadAssetModels();
	static void LoadAssetModel(const string& path);
	static void ProcessNode(aiNode* aiNode, const aiScene* aiScene, const string& dir);
	static void ProcessMesh(aiMesh* aMesh, const aiScene* aiScene, const string& dir);
	static void ProcessMaterialTextures(aiMaterial* aMat, aiTextureType aTextureType, const string& typeName, const string& dir);

	static bool GenerateGUIDFromPath(const string& assetPath, string& guid);

	static string GetGUIDFromAssetPath(const string& assetPath);
	static string GetAssetPathFromGUID(const string& guid);

private:
	static std::unordered_map<string, string> m_assetPath2GUID;
	static std::unordered_map<string, string> m_guid2AssetPath;


};

