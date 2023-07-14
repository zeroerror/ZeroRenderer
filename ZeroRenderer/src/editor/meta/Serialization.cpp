#include "Serialization.h"
#include "FileHelper.h"
#include "FileSuffix.h"
#include <sstream>
#include <iostream>

using namespace std;

void Serialization::SceneMeta_SerializeTo(const SceneMeta& sceneMeta, const string& path) {
	stringstream ss;
	ss << "GameObjects: " << endl;
	for (auto goMeta : sceneMeta.gameObjectMetas) {
		ss << "GameObject:" << endl;
		GameObjectMeta_SerializeTo(goMeta, ss);
	}
	ss << "---" << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	delete charResult;
	cout << "Scene Serialize. " << endl;

	{
		stringstream ss;
		ss << "guid: " << sceneMeta.guid << endl;
		string result = ss.str();
		size_t len = result.length() + 1;
		unsigned char* charResult = new unsigned char[len];
		memcpy(charResult, result.c_str(), len);
		string metaPath = metaPath + FileSuffix::SUFFIX_META;
		FileHelper::WriteCharsTo(metaPath, charResult);
		delete charResult;
		cout << "Scene Meta::Serialize | guid: " << sceneMeta.guid << endl;
	}
}

void Serialization::SceneMeta_DeserializeFrom(SceneMeta* sceneMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "GameObjects:") {
			while (getline(ss, line)) {
				iss = istringstream(line);
				if (!(iss >> key)) {
					break;
				}
				if (key == "GameObject:") {
					GameObjectMeta goMeta = GameObjectMeta();
					GameObjectMeta_DeserializeFrom(&goMeta, ss);
					sceneMeta->gameObjectMetas.push_back(goMeta);
				}
				else if (key == "---") {
					break;
				}
			}
		}
	}
	delete res;

	{
		unsigned char* res = new unsigned char[1024];
		string metaPath = path + FileSuffix::SUFFIX_META;
		FileHelper::ReadCharsFrom(metaPath, res);
		string str(reinterpret_cast<char*>(res));
		stringstream ss(str);
		string line;
		while (getline(ss, line)) {
			istringstream iss(line);
			string key;
			if (!(iss >> key)) {
				break;
			}
			if (key == "guid:") {
				iss >> sceneMeta->guid;
			}
		}
	}
}

void Serialization::PrefabMeta_SerializeTo(PrefabMeta& prefabMeta, const string& path) {
	stringstream ss;
	for (auto comMeta : prefabMeta.componentMetas) {
		ComponentMeta_SerializeTo(*comMeta, ss);
	}

	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	string prefabPath = path + FileSuffix::SUFFIX_PREFAB;
	FileHelper::WriteCharsTo(prefabPath, charResult);
	ss << "Prefab Serialize : " << prefabPath << endl;

	{
		stringstream ss;
		ss << "guid: " << prefabMeta.guid;

		string result = ss.str();
		size_t len = result.length() + 1;
		unsigned char* charResult = new unsigned char[len];
		memcpy(charResult, result.c_str(), len);
		string prefabMetaPath = prefabPath + FileSuffix::SUFFIX_META;
		FileHelper::WriteCharsTo(prefabMetaPath, charResult);
		ss << "Prefab Meta Serialize : " << prefabMetaPath << endl;
	}

}

void Serialization::PrefabMeta_DeserializeFrom(PrefabMeta* prefabMeta, const string& path) {
	if (!FileHelper::FileExist(path))return;
	unsigned char* res = new unsigned char[1024];

	string prefabPath = path.substr(0,path.find_last_of(".")) + FileSuffix::SUFFIX_PREFAB;
	FileHelper::ReadCharsFrom(prefabPath, res);
	string str(reinterpret_cast<char*>(res));

	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key != "componentType:") {
			break;
		}
		if (!(iss >> key)) {
			break;
		}
		;
		ComponentType_ comType = GetComponentType(key);
		if (ComponentType_Transform == comType) {
			TransformMeta transformMeta = prefabMeta->transformMeta;
			TransformMeta_DeserializeFrom(&transformMeta, ss);
		}
		else if (ComponentType_Camera == comType) {
			CameraMeta* cameraMeta = static_cast<CameraMeta*>(prefabMeta->AddComponentMeta<CameraMeta>());
			CameraMeta_DeserializeFrom(cameraMeta, ss);
		}
		else if (ComponentType_MeshFilter == comType) {
			MeshFilterMeta* meshFilterMeta = static_cast<MeshFilterMeta*>(prefabMeta->AddComponentMeta<MeshFilterMeta>());
			MeshFilterMeta_DeserializeFrom(meshFilterMeta, ss);
		}
		else if (ComponentType_MeshRenderer == comType) {
			MeshRendererMeta* meshRendererMeta = static_cast<MeshRendererMeta*>(prefabMeta->AddComponentMeta<MeshRendererMeta>());
			MeshRendererMeta_DeserializeFrom(meshRendererMeta, ss);
		}
		else if (ComponentType_SkinMeshRenderer == comType) {
			SkinMeshRendererMeta* skinMeshRendererMeta = static_cast<SkinMeshRendererMeta*>(prefabMeta->AddComponentMeta<SkinMeshRendererMeta>());
			SkinMeshRendererMeta_DeserializeFrom(skinMeshRendererMeta, ss);
		}
	}

	{
		string metaPath = prefabPath + FileSuffix::SUFFIX_META;
		FileHelper::ReadCharsFrom(metaPath, res);
		string str(reinterpret_cast<char*>(res));
		stringstream ss(str);
		string line;
		while (getline(ss, line)) {
			istringstream iss(line);
			string key;
			if (!(iss >> key)) {
				break;
			}
			if (key != "guid:") {
				break;
			}
			if (!(iss >> key)) {
				break;
			}

			prefabMeta->guid = key.c_str();
		}
	}
}

void Serialization::MaterialMeta_SerializeTo(const MaterialMeta& materialMeta, const string& path) {
	stringstream ss;
	ss << "shaderGUID: " << materialMeta.shaderGUID << endl;
	ss << "diffuseTextureGUID: " << materialMeta.diffuseTextureGUID << endl;
	ss << "specularTextureGUID: " << materialMeta.specularTextureGUID << endl;
	ss << "diffuseColor: " << materialMeta.diffuseColor.x << ' ' << materialMeta.diffuseColor.y << ' ' << materialMeta.diffuseColor.z << endl;
	ss << "specularIntensity: " << materialMeta.specularIntensity << endl;
	ss << "shininess: " << materialMeta.shininess << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	string matPath = path + FileSuffix::SUFFIX_MAT;
	FileHelper::WriteCharsTo(matPath, charResult);

	{
		stringstream ss;
		ss << "guid: " << materialMeta.guid << endl;
		string result = ss.str();
		size_t len = result.length() + 1;
		unsigned char* charResult = new unsigned char[len];
		memcpy(charResult, result.c_str(), len);
		string matMetaPath = matPath + FileSuffix::SUFFIX_META;
		FileHelper::WriteCharsTo(matMetaPath, charResult);
		cout << "Mat meta Serialize | guid: " << materialMeta.guid << endl;
	}
}

void Serialization::MaterialMeta_DeserializeFrom(MaterialMeta* materialMeta, const string& path) {

	string matPath = path + FileSuffix::SUFFIX_MAT;
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(matPath, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "shaderGUID:") {
			iss >> materialMeta->shaderGUID;
		}
		else if (key == "diffuseTextureGUID:") {
			iss >> materialMeta->diffuseTextureGUID;
		}
		else if (key == "specularTextureGUID:") {
			iss >> materialMeta->specularTextureGUID;
		}
		else if (key == "diffuseColor:") {
			iss >> key;
			materialMeta->diffuseColor.x = atoi(key.c_str());
			iss >> key;
			materialMeta->diffuseColor.y = atoi(key.c_str());
			iss >> key;
			materialMeta->diffuseColor.z = atoi(key.c_str());
		}
		else if (key == "specularIntensity:") {
			iss >> materialMeta->specularIntensity;
		}
		else if (key == "shininess:") {
			iss >> materialMeta->shininess;
		}
	}

	{
		string metaPath = path + FileSuffix::SUFFIX_META;
		unsigned char* res = new unsigned char[1024];
		FileHelper::ReadCharsFrom(metaPath, res);
		string str(reinterpret_cast<char*>(res));
		stringstream ss(str);
		string line;
		while (getline(ss, line)) {
			istringstream iss(line);
			string key;
			if (!(iss >> key)) {
				break;
			}
			if (key == "guid:") {
				iss >> materialMeta->guid;
			}
		}
		delete res;
	}
}

void Serialization::ShaderMeta_SerializeTo(ShaderMeta* shaderMeta, const string& path) {
	stringstream ss;
	ss << "guid: " << shaderMeta->guid << endl;
	ss << "useLightingMVP: " << shaderMeta->useLightingMVP << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	ss << "Shader Serialize | guid: " << shaderMeta->guid << endl;
}

void Serialization::ShaderMeta_DeserializeFrom(ShaderMeta* shaderMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> shaderMeta->guid;
		}
		else if (key == "useLightingMVP:") {
			iss >> shaderMeta->useLightingMVP;
		}
	}
}

void Serialization::TextureMeta_SerializeTo(const TextureMeta& textureMeta, const string& path) {
	stringstream ss;
	ss << "guid: " << textureMeta.guid << endl;
	ss << "width: " << textureMeta.width << endl;
	ss << "height: " << textureMeta.height << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
}

void Serialization::TextureMeta_DeserializeFrom(TextureMeta* textureMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> textureMeta->guid;
		}
		else if (key == "width:") {
			iss >> textureMeta->width;
		}
		else if (key == "height:") {
			iss >> textureMeta->height;
		}
	}
}

void Serialization::ObjMeta_SerializeTo(const ObjMeta& objMeta, const string& path) {
	stringstream ss;
	ss << "guid: " << objMeta.guid << endl;

	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	string objMetaPath = path + FileSuffix::SUFFIX_META;
	FileHelper::WriteCharsTo(objMetaPath, charResult);

	ss << "ObjMetadata::Serialize | guid: " << objMeta.guid << endl;
}

void Serialization::ObjMeta_DeserializeFrom(ObjMeta* objMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> objMeta->guid;
		}
	}
}

void Serialization::GameObjectMeta_SerializeTo(const GameObjectMeta& gameObjectMeta, stringstream& ss) {
	TransformMeta_SerializeTo(gameObjectMeta.transformMeta, ss);
	for (auto comMeta : gameObjectMeta.componentMetas) {
		ComponentMeta_SerializeTo(*comMeta, ss);
	}
}

void Serialization::GameObjectMeta_DeserializeFrom(GameObjectMeta* gameObjectMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key != "componentType:") {
			break;
		}
		if (!(iss >> key)) {
			break;
		}
		if (ComponentType_Transform == (ComponentType_)atoi(key.c_str())) {
			TransformMeta_DeserializeFrom(&gameObjectMeta->transformMeta, ss);
		}
		else if (ComponentType_Camera == (ComponentType_)atoi(key.c_str())) {
			CameraMeta* cameraMeta = gameObjectMeta->AddComponentMeta<CameraMeta>();
			CameraMeta_DeserializeFrom(cameraMeta, ss);
		}
	}
}

void Serialization::TransformMeta_SerializeTo(const TransformMeta& transformMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[transformMeta.componentType] << endl;
	ss << "position: " << transformMeta.position.x << ' ' << transformMeta.position.y << ' ' << transformMeta.position.z << endl;
	ss << "rotation: " << transformMeta.rotation.x << ' ' << transformMeta.rotation.y << ' ' << transformMeta.rotation.z << ' ' << transformMeta.rotation.w << endl;
	ss << "componentEnd" << endl;
}

void Serialization::TransformMeta_DeserializeFrom(TransformMeta* transformMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "componentEnd") {
			break;
		}
		if (key == "position:") {
			iss >> key;
			transformMeta->position.x = atoi(key.c_str());
			iss >> key;
			transformMeta->position.y = atoi(key.c_str());
			iss >> key;
			transformMeta->position.z = atoi(key.c_str());
		}
		else if (key == "rotation:") {
			iss >> key;
			transformMeta->rotation.x = atoi(key.c_str());
			iss >> key;
			transformMeta->rotation.y = atoi(key.c_str());
			iss >> key;
			transformMeta->rotation.z = atoi(key.c_str());
			iss >> key;
			transformMeta->rotation.w = atoi(key.c_str());
		}
	}
}

void Serialization::CameraMeta_SerializeTo(const CameraMeta& cameraMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[cameraMeta.componentType] << endl;
	ss << "scrWidth: " << cameraMeta.scrWidth << endl;
	ss << "scrHeight: " << cameraMeta.scrHeight << endl;
	ss << "fov: " << cameraMeta.fov << endl;
	ss << "nearPlane: " << cameraMeta.nearPlane << endl;
	ss << "farPlane: " << cameraMeta.farPlane << endl;
	ss << "orthoSize: " << cameraMeta.orthoSize << endl;
	ss << "componentEnd" << endl;
}

void Serialization::CameraMeta_DeserializeFrom(CameraMeta* cameraMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "componentEnd") {
			break;
		}
		if (key == "scrWidth:") {
			iss >> key;
			cameraMeta->scrWidth = atof(key.c_str());
		}
		else if (key == "scrHeight:") {
			iss >> key;
			cameraMeta->scrHeight = atof(key.c_str());
		}
		else if (key == "fov:") {
			iss >> key;
			cameraMeta->fov = atof(key.c_str());
		}
		else if (key == "nearPlane:") {
			iss >> key;
			cameraMeta->nearPlane = atof(key.c_str());
		}
		else if (key == "farPlane:") {
			iss >> key;
			cameraMeta->farPlane = atof(key.c_str());
		}
		else if (key == "orthoSize:") {
			iss >> key;
			cameraMeta->orthoSize = atof(key.c_str());
		}
	}
}

void Serialization::ComponentMeta_SerializeTo(const ComponentMeta& componentMeta, stringstream& ss) {
	ComponentType_ comType = componentMeta.componentType;
	if (comType == ComponentType_Transform)TransformMeta_SerializeTo(static_cast<TransformMeta>(componentMeta), ss);
	else if (comType == ComponentType_Camera)CameraMeta_SerializeTo(static_cast<CameraMeta>(componentMeta), ss);
	else if (comType == ComponentType_MeshFilter)MeshFilterMeta_SerializeTo(static_cast<const MeshFilterMeta&>(componentMeta), ss);
	else if (comType == ComponentType_MeshRenderer)MeshRendererMeta_SerializeTo(static_cast<const MeshRendererMeta&>(componentMeta), ss);
	else if (comType == ComponentType_SkinMeshRenderer)SkinMeshRendererMeta_SerializeTo(static_cast<const SkinMeshRendererMeta&>(componentMeta), ss);
}

void Serialization::SkinMeshRendererMeta_SerializeTo(const SkinMeshRendererMeta& skinMeshRendererMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[skinMeshRendererMeta.componentType] << endl;
	for (auto meshFilterMeta : skinMeshRendererMeta.meshFilterMetas) {
		MeshFilterMeta_SerializeTo(*meshFilterMeta, ss);
	}
	for (auto meshRendererMeta : skinMeshRendererMeta.meshRendererMetas) {
		MeshRendererMeta_SerializeTo(*meshRendererMeta, ss);
	}
	ss << "componentEnd" << endl;
}

void Serialization::SkinMeshRendererMeta_DeserializeFrom(SkinMeshRendererMeta* skinMeshRendererMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key))break;
		if (key == "componentEnd")	break;
		if (!(iss >> key))break;

		if (key == ComponentType_Names[ComponentType_MeshFilter]) {
			MeshFilterMeta* meshFilterMeta = new MeshFilterMeta();
			MeshFilterMeta_DeserializeFrom(meshFilterMeta, ss);
			skinMeshRendererMeta->meshFilterMetas.push_back(meshFilterMeta);
		}
		else if (key == ComponentType_Names[ComponentType_MeshRenderer]) {
			MeshRendererMeta* meshRendererMeta = new MeshRendererMeta();
			MeshRendererMeta_DeserializeFrom(meshRendererMeta, ss);
			skinMeshRendererMeta->meshRendererMetas.push_back(meshRendererMeta);
		}
	}
}

void Serialization::MeshFilterMeta_SerializeTo(const MeshFilterMeta& meshFilterMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[ComponentType_MeshFilter] << endl;
	ss << "modelGUID: " << meshFilterMeta.modelGUID << endl;
	ss << "meshIndex: " << meshFilterMeta.meshIndex << endl;
	ss << "componentEnd" << endl;
}

void Serialization::MeshFilterMeta_DeserializeFrom(MeshFilterMeta* meshFilterMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "componentEnd") {
			break;
		}
		if (key == "modelGUID:") {
			iss >> meshFilterMeta->modelGUID;
		}
		else if (key == "meshIndex:") {
			iss >> key;
			meshFilterMeta->meshIndex = atoi(key.c_str());
		}
	}
}

void Serialization::MeshRendererMeta_SerializeTo(const MeshRendererMeta& meshRendererMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[meshRendererMeta.componentType] << endl;
	ss << "materialGUID: " << meshRendererMeta.materialGUID << endl;
	ss << "componentEnd" << endl;
}

void Serialization::MeshRendererMeta_DeserializeFrom(MeshRendererMeta* meshRendererMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "componentEnd") {
			break;
		}
		if (key == "materialGUID:") {
			iss >> meshRendererMeta->materialGUID;
		}
	}
}
