#pragma once

#include <unordered_map>
#include <string>
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Database {

public:
	static Material* defaultMaterial;
	static Material* defaultLightMaterial;
	static Material* lightCubeMaterial;
	static Material* depthMapMaterial;
	static void LoadDatabase();
	static void LoadMaterials();
	static void LoadAssetModels();
	static void LoadAssetModel(const std::string& path);
	static void ProcessNode(aiNode* aiNode, const aiScene* aiScene, const std::string& dir);
	static void ProcessMesh(aiMesh* aMesh, const aiScene* aiScene, const std::string& dir);
	static void ProcessMaterialTextures(aiMaterial* aMat, aiTextureType aTextureType, const std::string& typeName, const std::string& dir);

	static void LoadAssetTextures();
	static void LoadAssetTexture(const std::string& texturePath1);
	static void LoadAssetShaders();
	static void LoadAssetShader(const std::string& assetPath);
	static bool GenerateGUIDFromPath(const std::string& assetPath,std::string& guid);

	static std::string GetGUIDFromAssetPath(const std::string& assetPath);
	static std::string GetAssetPathFromGUID(const std::string& guid);

private:
	static std::unordered_map<std::string, std::string> m_assetPath2GUID;
	static std::unordered_map<std::string, std::string> m_guid2AssetPath;

};

