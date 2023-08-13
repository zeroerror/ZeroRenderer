#include "EditorDatabase.h"

#include <vector>
#include <random>

#include <iostream>
#include <sstream>
#include <filesystem>
#include "FileSuffix.h"
#include "FileHelper.h"
#include <assimp/postprocess.h>

#include "DirectLightMeta.h"
#include "TextureMeta.h"
#include "ShaderMeta.h"
#include "PrefabInstanceMeta.h"
#include "TextureSlotCollection.h"
#include "Serialization.h"
#include "EditorModelManager.h"
#include "EditorDefaultConfig.h"

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
			string metaPath = assetPath + FileSuffix::SUFFIX_META;
			string extensionStr = path.extension().string();
			if (extensionStr == FileSuffix::SUFFIX_PNG) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(metaPath)) {
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
				std::cout << " +++ EditorDatabase: Import " << assetPath << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_SHADER) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(metaPath)) {
					ShaderMeta shaderMeta = ShaderMeta();
					shaderMeta.guid = guid;
					ShaderMeta_SerializeTo(shaderMeta, assetPath);
				}
				else {
					ShaderMeta shaderMeta = ShaderMeta();
					ShaderMeta_DeserializeFrom(&shaderMeta, assetPath);
					guid = shaderMeta.guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_MAT) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(metaPath)) {
					MaterialMeta materialMeta = MaterialMeta();
					materialMeta.guid = guid;
					MaterialMeta_SerializeTo(materialMeta, assetPath);
				}
				else {
					MaterialMeta materialMeta = MaterialMeta();
					MaterialMeta_DeserializeFrom(&materialMeta, assetPath);
					materialMeta.guid = guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_OBJ) {
				EditorDatabase::ImportModel(assetPath);
			}
			else if (extensionStr == FileSuffix::SUFFIX_PREFAB) {
				string guid = GenerateGUIDFromAssetPath(assetPath);
				InsertToMap_AssetPath2GUID(assetPath, guid);
				InsertToMap_GUID2AssetPath(guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << guid << std::endl;
			}
			else if (extensionStr == FileSuffix::SUFFIX_SCENE) {
				SceneMeta sceneMeta = SceneMeta();
				string guid = GenerateGUIDFromAssetPath(assetPath);
				if (!FileHelper::FileExist(metaPath)) {
					sceneMeta.guid = guid;
					SceneMeta_SerializeTo(sceneMeta, assetPath);
				}
				else {
					SceneMeta_DeserializeFrom(&sceneMeta, assetPath);
					sceneMeta.guid = guid;
				}
				InsertToMap_AssetPath2GUID(assetPath, sceneMeta.guid);
				InsertToMap_GUID2AssetPath(sceneMeta.guid, assetPath);
				std::cout << " +++ EditorDatabase: Import " << assetPath << sceneMeta.guid << std::endl;
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

	const aiScene* aScene = nullptr;
	if (!EditorModelManager::TryLoadModel(assetPath, aScene)) {
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
	if (pos != string::npos) {
		string prefabPath = assetPath.substr(0, pos) + FileSuffix::SUFFIX_PREFAB;
		PrefabMeta_SerializeTo(prefabMeta, prefabPath);
	}
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
	string matPath = dir + meshName + FileSuffix::SUFFIX_MAT;
	string materialGUID = GenerateGUIDFromAssetPath(matPath);
	materialMeta.guid = materialGUID;
	materialMeta.shaderGUID = "4fa3f76955080b44";

	MaterialMeta_SerializeTo(materialMeta, matPath);

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

void EditorDatabase::ClearFile(const unsigned int& suffixFlag) {
	vector<string> filePaths;
	FileHelper::GetFilePaths("asset", suffixFlag, filePaths);
	for (auto path : filePaths) {
		FileHelper::DeleteFile(path);
	}
}

void EditorDatabase::ClearFile(const string& dir, const unsigned int& suffixFlag) {
	vector<string> filePaths;
	FileHelper::GetFilePaths(dir, suffixFlag, filePaths);
	for (auto path : filePaths) {
		FileHelper::DeleteFile(path);
	}
}

bool EditorDatabase::SetMat_TextureGUID(const string& matPath, const string& texturePath, const unsigned int& textureSlot) {
	if (!AssetPathExist(texturePath)) return false;
	if (!AssetPathExist(matPath)) return false;

	string texGUID;
	if (!TryGetGUIDFromAssetPath(texturePath, texGUID)) {
		return false;
	}

	MaterialMeta materialMeta = MaterialMeta();
	MaterialMeta_DeserializeFrom(&materialMeta, matPath);
	if (textureSlot == TEX_SLOT_DIFFUSE_MAP)materialMeta.diffuseTextureGUID = texGUID;
	else if (textureSlot == TEX_SLOT_SPECULAR_MAP)materialMeta.specularTextureGUID = texGUID;
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

string EditorDatabase::GenerateGUIDFromAssetPath(const string& assetPath) {
	string norAssetPath = FileHelper::NormalizedPath(assetPath);
	std::hash<string> hasher;
	stringstream ss;

	// 生成路径的哈希值
	std::size_t pathHash = hasher(norAssetPath);
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

void EditorDatabase::GenerateDefaultSceneMeta() {
	unsigned int scrWidth = 1920;
	unsigned int scrHeight = 1080;

	string defaultScenePath = EditorDefaultConfig::DefaultScenePath();
	string defaultSceneMetaPath = defaultScenePath + FileSuffix::SUFFIX_META;
	string sceneGUID = GenerateGUIDFromAssetPath(defaultScenePath);
	SceneMeta sceneMeta = SceneMeta();
	sceneMeta.guid = sceneGUID;

	string defaultMatGUID = GenerateGUIDFromAssetPath("asset/material/default.mat");
	string defaultLightMatGUID = GenerateGUIDFromAssetPath("asset/material/defaultLight.mat");
	string lightCubeMatGUID = GenerateGUIDFromAssetPath("asset/material/lightCube.mat");
	string depthMapMatGUID = GenerateGUIDFromAssetPath("asset/material/depthMap.mat");

	int gid = 1;
	// ======================== Scene
	GameObjectMeta* cameraGOMeta = new GameObjectMeta();
	cameraGOMeta->name = "Camera";
	CameraMeta* cameraMeta = cameraGOMeta->AddComponentMeta<CameraMeta>();
	cameraMeta->scrWidth = scrWidth;
	cameraMeta->scrHeight = scrHeight;
	cameraGOMeta->transformMeta->position = vec3(0, 20, -10);
	cameraGOMeta->transformMeta->rotation = quat(vec3(radians(30.0f), radians(0.0f), radians(0.0f)));
	cameraGOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(cameraGOMeta);

	MeshFilterMeta* meshFilterMeta;
	MeshRendererMeta* meshRendererMeta;

	GameObjectMeta* directLightGOMeta = new GameObjectMeta();
	directLightGOMeta->name = "DirectLight";
	DirectLightMeta* directLightMeta = directLightGOMeta->AddComponentMeta<DirectLightMeta>();
	directLightMeta->scrWidth = scrWidth;
	directLightMeta->scrHeight = scrHeight;
	directLightMeta->shadowType = ShadowType::Hard;
	directLightGOMeta->transformMeta->position = vec3(0, 10.0f, 0);
	directLightGOMeta->transformMeta->rotation = quat(vec3(0, 0.5f, 0));
	directLightMeta->fov = cameraMeta->fov;
	directLightMeta->scrWidth = cameraMeta->scrWidth;
	directLightMeta->scrHeight = cameraMeta->scrHeight;
	directLightMeta->nearPlane = cameraMeta->nearPlane;
	directLightMeta->farPlane = cameraMeta->farPlane;
	directLightMeta->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	directLightGOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(directLightGOMeta);

	// Create a depth map 2D image. 
	//GameObjectMeta* depthMapImageGOMeta = new GameObjectMeta();
	//depthMapImageGOMeta->name = "DepthMapImage";
	//depthMapImageGOMeta->transformMeta->position = vec3(0.0f, 10.0f, 20.0f);
	//depthMapImageGOMeta->transformMeta->rotation = vec3(0.0f, radians(180.0f), 0.0f);
	//meshFilterMeta = cameraGOMeta->AddComponentMeta<MeshFilterMeta>();
	//meshRendererMeta = cameraGOMeta->AddComponentMeta<MeshRendererMeta>();
	//meshRendererMeta->materialGUID = depthMapMatGUID;
	//sceneMeta.gameObjectMetas.push_back(depthMapImageGOMeta);

	//// ---- Create a central light source cube
	//GameObjectMeta* lightCubeGOMeta = new GameObjectMeta();
	//lightCubeGOMeta->name = "LightCube";
	//lightCubeGOMeta->transformMeta->scale = vec3(0.2f, 0.2f, 1.0f);
	//meshFilterMeta = lightCubeGOMeta->AddComponentMeta<MeshFilterMeta>();
	//meshRendererMeta = lightCubeGOMeta->AddComponentMeta<MeshRendererMeta>();
	//meshRendererMeta->materialGUID = lightCubeMatGUID;
	//sceneMeta.gameObjectMetas.push_back(lightCubeGOMeta);

	// ---- Create the ground
	GameObjectMeta* groundCubeGOMeta = new GameObjectMeta();
	groundCubeGOMeta->name = "GroundCube";
	groundCubeGOMeta->transformMeta->position = vec3(0.0f, -0.05f, 0.0f);
	groundCubeGOMeta->transformMeta->scale = vec3(20.0f, 0.1f, 30.0f);
	meshFilterMeta = groundCubeGOMeta->AddComponentMeta<MeshFilterMeta>();
	meshFilterMeta->meshIndex = -1001;
	meshRendererMeta = groundCubeGOMeta->AddComponentMeta<MeshRendererMeta>();
	meshRendererMeta->materialGUID = defaultLightMatGUID;
	groundCubeGOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(groundCubeGOMeta);

	// ---- Create wall 1
	GameObjectMeta* wall1GOMeta = new GameObjectMeta();
	wall1GOMeta->name = "Wall1";
	wall1GOMeta->transformMeta->position = vec3(-8.0f, 2.5f, 0.0f);
	wall1GOMeta->transformMeta->scale = vec3(1.0f, 5.0f, 10.0f);
	meshFilterMeta = wall1GOMeta->AddComponentMeta<MeshFilterMeta>();
	meshFilterMeta->meshIndex = -1001;
	meshRendererMeta = wall1GOMeta->AddComponentMeta<MeshRendererMeta>();
	meshRendererMeta->materialGUID = defaultLightMatGUID;
	wall1GOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(wall1GOMeta);

	// ---- Create wall 2
	GameObjectMeta* wall2GOMeta = new GameObjectMeta();
	wall2GOMeta->name = "Wall2";
	wall2GOMeta->transformMeta->position = vec3(0.0f, 2.5f, -8.0f);
	wall2GOMeta->transformMeta->scale = vec3(10.0f, 5.0f, 1.0f);
	meshFilterMeta = wall2GOMeta->AddComponentMeta<MeshFilterMeta>();
	meshFilterMeta->meshIndex = -1001;
	meshRendererMeta = wall2GOMeta->AddComponentMeta<MeshRendererMeta>();
	meshRendererMeta->materialGUID = defaultLightMatGUID;
	wall2GOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(wall2GOMeta);

	// Create obstacles 1
	GameObjectMeta* obstacle1GOMeta = new GameObjectMeta();
	obstacle1GOMeta->name = "obstacle1";
	obstacle1GOMeta->transformMeta->position = vec3(-4.0f, 1.0f, 4.0f);
	obstacle1GOMeta->transformMeta->scale = vec3(2.0f, 2.0f, 2.0f);
	meshFilterMeta = obstacle1GOMeta->AddComponentMeta<MeshFilterMeta>();
	meshFilterMeta->meshIndex = -1001;
	meshRendererMeta = obstacle1GOMeta->AddComponentMeta<MeshRendererMeta>();
	meshRendererMeta->materialGUID = defaultLightMatGUID;
	obstacle1GOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(obstacle1GOMeta);

	// Create obstacles 2
	GameObjectMeta* obstacle2GOMeta = new GameObjectMeta();
	obstacle2GOMeta->name = "obstacle2";
	obstacle2GOMeta->transformMeta->position = vec3(4.0f, 1.0f, -4.0f);
	obstacle2GOMeta->transformMeta->scale = vec3(2.0f, 2.0f, 2.0f);
	meshFilterMeta = obstacle2GOMeta->AddComponentMeta<MeshFilterMeta>();
	meshFilterMeta->meshIndex = -1001;
	meshRendererMeta = obstacle2GOMeta->AddComponentMeta<MeshRendererMeta>();
	meshRendererMeta->materialGUID = defaultLightMatGUID;
	obstacle2GOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(obstacle2GOMeta);

	// Create obstacles 3
	GameObjectMeta* obstacle3GOMeta = new GameObjectMeta();
	obstacle3GOMeta->name = "obstacle3";
	obstacle3GOMeta->transformMeta->position = vec3(6.0f, 0.5f, 6.0f);
	obstacle3GOMeta->transformMeta->scale = vec3(3.0f, 1.0f, 2.0f);
	meshFilterMeta = obstacle3GOMeta->AddComponentMeta<MeshFilterMeta>();
	meshFilterMeta->meshIndex = -1001;
	meshRendererMeta = obstacle3GOMeta->AddComponentMeta<MeshRendererMeta>();
	meshRendererMeta->materialGUID = defaultLightMatGUID;
	obstacle3GOMeta->gid = gid++;
	sceneMeta.gameObjectMetas.push_back(obstacle3GOMeta);

	string prefabGUID;
	if (TryGetGUIDFromAssetPath("asset/model/nanosuit/nanosuit.prefab", prefabGUID)) {
		PrefabInstanceMeta* prefabInstanceMeta = new PrefabInstanceMeta();
		prefabInstanceMeta->guid = prefabGUID;
		prefabInstanceMeta->gameObjectMeta->name = "Nanosuit";
		prefabInstanceMeta->gameObjectMeta->gid= gid++;
		prefabInstanceMeta->gameObjectMeta->transformMeta->position = vec3(0, 0, 0);
		prefabInstanceMeta->gameObjectMeta->transformMeta->rotation = quat(vec3(radians(0.0f), radians(0.0f), radians(0.0f)));
		prefabInstanceMeta->gameObjectMeta->transformMeta->scale = vec3(0.25f, 0.25f, 0.25f);
		sceneMeta.prefabInstanceMetas.push_back(prefabInstanceMeta);
	}

	GameObjectMeta* rootGOMeta = new GameObjectMeta();
	rootGOMeta->name = "Root";
	rootGOMeta->gid = gid++;
	for(auto goMeta : sceneMeta.gameObjectMetas){
		rootGOMeta->transformMeta->AddChild(goMeta->transformMeta);
	}
	for(auto prefabInstanceMeta : sceneMeta.prefabInstanceMetas){
		rootGOMeta->transformMeta->AddChild(prefabInstanceMeta->gameObjectMeta->transformMeta);
	}
	sceneMeta.gameObjectMetas.push_back(rootGOMeta);

	SceneMeta_SerializeTo(sceneMeta, EditorDefaultConfig::DefaultScenePath());
}

void EditorDatabase::GenerateDefaultShader() {
	string defaultShaderPath = EditorDefaultConfig::DefaultShaderPath();
	ShaderMeta shaderMeta = ShaderMeta();
	shaderMeta.guid = GenerateGUIDFromAssetPath(defaultShaderPath);
	shaderMeta.uniforms = vector<ShaderUniform>();
	vector<ShaderUniform>& uniforms = shaderMeta.uniforms;

	ShaderUniform uniform = ShaderUniform();
	uniform.name = "testInt";
	uniform.type = ShaderUniformType_Int;
	uniform.value = 1;
	uniforms.push_back(uniform);

	uniform.name = "testFloat";
	uniform.type = ShaderUniformType_Float;
	uniform.value = 0.66f;
	uniforms.push_back(uniform);

	uniform.name = "testFloat3";
	uniform.type = ShaderUniformType_Float3;
	uniform.value = vec3(0.33f, 0.33f, 0.33f);
	uniforms.push_back(uniform);

	uniform.name = "u_mixedColor";
	uniform.type = ShaderUniformType_Float4;
	uniform.value = vec4(0.33f, 0.33f, 0.33f, 1.0f);
	uniforms.push_back(uniform);

	ShaderMeta_SerializeTo(shaderMeta, defaultShaderPath);

}
