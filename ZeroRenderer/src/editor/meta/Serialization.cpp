#include "Serialization.h"
#include "FileHelper.h"
#include "FileSuffix.h"
#include "EditorDefaultConfig.h"
#include <sstream>
#include <iostream>

using namespace std;

void Serialization::GameObjectMeta_SerializeTo(const GameObjectMeta& gameObjectMeta, stringstream& ss) {
	ss << EditorDefaultConfig::DefaultGameObjectStartStr() << endl;
	ss << "name: " << gameObjectMeta.name << endl;
	ss << endl;
	ss << EditorDefaultConfig::DefaultComponentStartStr() << endl;
	TransformMeta_SerializeTo(gameObjectMeta.transformMeta, ss);
	ss << EditorDefaultConfig::DefaultComponentEndStr() << endl;
	ss << endl;

	for (auto comMeta : gameObjectMeta.componentMetas) {
		ComponentMeta_SerializeTo(*comMeta, ss);
	}
	ss << EditorDefaultConfig::DefaultGameObjectEndStr() << endl;
}

void Serialization::GameObjectMeta_DeserializeFrom(GameObjectMeta* gameObjectMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;

		if (!(iss >> key)) continue;
		if (key == EditorDefaultConfig::DefaultGameObjectEndStr()) break;

		if (key == "name:") {
			iss >> key;
			gameObjectMeta->name = key;
			continue;
		}

		if (key != "componentType:")continue;

		iss >> key;
		ComponentType_ comType = GetComponentType(key);
		if (ComponentType_Transform == comType) {
			TransformMeta_DeserializeFrom(&gameObjectMeta->transformMeta, ss);
			continue;
		}

		if (ComponentType_Camera == comType) {
			CameraMeta* cameraMeta = gameObjectMeta->AddComponentMeta<CameraMeta>();
			CameraMeta_DeserializeFrom(cameraMeta, ss);
			continue;
		}

		if (ComponentType_DirectLight == comType) {
			DirectLightMeta* directLightMeta = gameObjectMeta->AddComponentMeta<DirectLightMeta>();
			DirectLightMeta_DeserializeFrom(directLightMeta, ss);
			continue;
		}

		if (ComponentType_MeshRenderer == comType) {
			MeshRendererMeta* meshRendererMeta = gameObjectMeta->AddComponentMeta<MeshRendererMeta>();
			MeshRendererMeta_DeserializeFrom(meshRendererMeta, ss);
			continue;
		}

		if (ComponentType_MeshFilter == comType) {
			MeshFilterMeta* meshFilterMeta = gameObjectMeta->AddComponentMeta<MeshFilterMeta>();
			MeshFilterMeta_DeserializeFrom(meshFilterMeta, ss);
			continue;
		}

		cout << "################ Don't know what this component type is!!! Please check!!! comType: " << comType << endl;
	}
}

void Serialization::TransformMeta_SerializeTo(const TransformMeta& transformMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[transformMeta.componentType] << endl;
	ss << "position: " << transformMeta.position.x << ' ' << transformMeta.position.y << ' ' << transformMeta.position.z << endl;
	ss << "rotation: " << transformMeta.rotation.x << ' ' << transformMeta.rotation.y << ' ' << transformMeta.rotation.z << ' ' << transformMeta.rotation.w << endl;
}

void Serialization::TransformMeta_DeserializeFrom(TransformMeta* transformMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == EditorDefaultConfig::DefaultComponentEndStr()) {
			break;
		}
		if (key == "position:") {
			iss >> key;
			transformMeta->position.x = atof(key.c_str());
			iss >> key;
			transformMeta->position.y = atof(key.c_str());
			iss >> key;
			transformMeta->position.z = atof(key.c_str());
		}
		else if (key == "rotation:") {
			iss >> key;
			transformMeta->rotation.x = atof(key.c_str());
			iss >> key;
			transformMeta->rotation.y = atof(key.c_str());
			iss >> key;
			transformMeta->rotation.z = atof(key.c_str());
			iss >> key;
			transformMeta->rotation.w = atof(key.c_str());
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
}

void Serialization::CameraMeta_DeserializeFrom(CameraMeta* cameraMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == EditorDefaultConfig::DefaultComponentEndStr()) {
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

void Serialization::DirectLightMeta_SerializeTo(const DirectLightMeta& directLightMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[directLightMeta.componentType] << endl;
	ss << "color: " << directLightMeta.color.x << ' ' << directLightMeta.color.y << ' ' << directLightMeta.color.z << endl;
	ss << "shadowType: " << static_cast<int>(directLightMeta.shadowType) << endl;
	ss << "scrWidth: " << directLightMeta.scrWidth << endl;
	ss << "scrHeight: " << directLightMeta.scrHeight << endl;
	ss << "fov: " << directLightMeta.fov << endl;
	ss << "nearPlane: " << directLightMeta.nearPlane << endl;
	ss << "farPlane: " << directLightMeta.farPlane << endl;
}

void Serialization::DirectLightMeta_DeserializeFrom(DirectLightMeta* directLightMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == EditorDefaultConfig::DefaultComponentEndStr()) {
			break;
		}
		if (key == "color:") {
			iss >> key;
			directLightMeta->color.x = atof(key.c_str());
			iss >> key;
			directLightMeta->color.y = atof(key.c_str());
			iss >> key;
			directLightMeta->color.z = atof(key.c_str());
		}
		else if (key == "shadowType:") {
			iss >> key;
			directLightMeta->shadowType = (ShadowType)(atoi(key.c_str()));
		}
		else if (key == "scrWidth:") {
			iss >> key;
			directLightMeta->scrWidth = atof(key.c_str());
		}
		else if (key == "scrHeight:") {
			iss >> key;
			directLightMeta->scrHeight = atof(key.c_str());
		}
		else if (key == "fov:") {
			iss >> key;
			directLightMeta->fov = atof(key.c_str());
		}
		else if (key == "nearPlane:") {
			iss >> key;
			directLightMeta->nearPlane = atof(key.c_str());
		}
		else if (key == "farPlane:") {
			iss >> key;
			directLightMeta->farPlane = atof(key.c_str());
		}
	}
}

void Serialization::SkinMeshRendererMeta_SerializeTo(const SkinMeshRendererMeta& skinMeshRendererMeta, stringstream& ss) {
	ss << "componentType: " << ComponentType_Names[skinMeshRendererMeta.componentType] << endl;
	for (auto meshFilterMeta : skinMeshRendererMeta.meshFilterMetas) {
		MeshFilterMeta_SerializeTo(*meshFilterMeta, ss);
	}
	for (auto meshRendererMeta : skinMeshRendererMeta.meshRendererMetas) {
		MeshRendererMeta_SerializeTo(*meshRendererMeta, ss);
	}
}

void Serialization::SkinMeshRendererMeta_DeserializeFrom(SkinMeshRendererMeta* skinMeshRendererMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key))break;
		if (key == EditorDefaultConfig::DefaultComponentEndStr())	break;
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
	ss << EditorDefaultConfig::DefaultComponentEndStr() << endl;
}

void Serialization::MeshFilterMeta_DeserializeFrom(MeshFilterMeta* meshFilterMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == EditorDefaultConfig::DefaultComponentEndStr()) {
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
	ss << EditorDefaultConfig::DefaultComponentEndStr() << endl;
}

void Serialization::MeshRendererMeta_DeserializeFrom(MeshRendererMeta* meshRendererMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == EditorDefaultConfig::DefaultComponentEndStr()) {
			break;
		}
		if (key == "materialGUID:") {
			iss >> meshRendererMeta->materialGUID;
		}
	}
}

void Serialization::ComponentMeta_SerializeTo(const ComponentMeta& componentMeta, stringstream& ss) {
	ss << EditorDefaultConfig::DefaultComponentStartStr() << endl;
	ComponentType_ comType = componentMeta.componentType;
	if (comType == ComponentType_Transform)TransformMeta_SerializeTo(static_cast<const TransformMeta&>(componentMeta), ss);
	else if (comType == ComponentType_Camera)CameraMeta_SerializeTo(static_cast<const CameraMeta&>(componentMeta), ss);
	else if (comType == ComponentType_DirectLight)DirectLightMeta_SerializeTo(static_cast<const DirectLightMeta&>(componentMeta), ss);
	else if (comType == ComponentType_MeshFilter)MeshFilterMeta_SerializeTo(static_cast<const MeshFilterMeta&>(componentMeta), ss);
	else if (comType == ComponentType_MeshRenderer)MeshRendererMeta_SerializeTo(static_cast<const MeshRendererMeta&>(componentMeta), ss);
	else if (comType == ComponentType_SkinMeshRenderer)SkinMeshRendererMeta_SerializeTo(static_cast<const SkinMeshRendererMeta&>(componentMeta), ss);
	ss << EditorDefaultConfig::DefaultComponentEndStr() << endl;
}

void Serialization::PrefabInstanceMeta_SerializeTo(PrefabInstanceMeta& prefabInstanceMeta, stringstream& ss) {
	ss << EditorDefaultConfig::DefaultPrefabInstanceStartStr() << endl;
	ss << "guid: " << prefabInstanceMeta.guid << endl;
	ComponentMeta_SerializeTo(prefabInstanceMeta.transformMeta, ss);
	ss << EditorDefaultConfig::DefaultPrefabInstanceEndStr() << endl;
}

void Serialization::PrefabInstanceMeta_DeserializeFrom(PrefabInstanceMeta* prefabInstanceMeta, stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;

		if (!(iss >> key)) continue;
		if (key == EditorDefaultConfig::DefaultPrefabInstanceEndStr()) break;

		if (key == "guid:") {
			iss >> key;
			prefabInstanceMeta->guid = key;
			continue;
		}

		if (key != "componentType:")continue;

		iss >> key;

		ComponentType_ comType = GetComponentType(key);
		if (ComponentType_Transform == comType) {
			TransformMeta_DeserializeFrom(&prefabInstanceMeta->transformMeta, ss);
			continue;
		}
	}
}

void Serialization::PrefabMeta_SerializeTo(const PrefabMeta& prefabMeta, const string& path) {
	stringstream ss;
	for (auto comMeta : prefabMeta.componentMetas) {
		ComponentMeta_SerializeTo(*comMeta, ss);
	}

	string result = ss.str();
	size_t len = result.length() + 1;
	char* charResult = new char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	ss << "Prefab Serialize : " << path << endl;

	{
		stringstream ss;
		ss << "guid: " << prefabMeta.guid;
		ss << "name: " << prefabMeta.name;

		string result = ss.str();
		size_t len = result.length() + 1;
		char* charResult = new char[len];
		memcpy(charResult, result.c_str(), len);
		string prefabMetaPath = path + FileSuffix::SUFFIX_META;
		FileHelper::WriteCharsTo(prefabMetaPath, charResult);
		ss << "Prefab Meta Serialize : " << prefabMetaPath << endl;
	}

}

void Serialization::PrefabMeta_DeserializeFrom(PrefabMeta& prefabMeta, const string& path) {
	unsigned int charCount = FileHelper::GetFileCharSize(path);
	if (charCount == 0) {
		cout << "############### no bytes. path: " << path << endl;
		return;
	}

	char* res = new char[charCount];

	FileHelper::ReadCharsFrom(path, res);
	string str(res);

	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key != "componentType:") {
			continue;
		}
		if (!(iss >> key)) {
			break;
		}
		;
		ComponentType_ comType = GetComponentType(key);
		if (ComponentType_Transform == comType) {
			TransformMeta transformMeta = prefabMeta.transformMeta;
			TransformMeta_DeserializeFrom(&transformMeta, ss);
		}
		else if (ComponentType_Camera == comType) {
			CameraMeta* cameraMeta = static_cast<CameraMeta*>(prefabMeta.AddComponentMeta<CameraMeta>());
			CameraMeta_DeserializeFrom(cameraMeta, ss);
		}
		else if (ComponentType_MeshFilter == comType) {
			MeshFilterMeta* meshFilterMeta = static_cast<MeshFilterMeta*>(prefabMeta.AddComponentMeta<MeshFilterMeta>());
			MeshFilterMeta_DeserializeFrom(meshFilterMeta, ss);
		}
		else if (ComponentType_MeshRenderer == comType) {
			MeshRendererMeta* meshRendererMeta = static_cast<MeshRendererMeta*>(prefabMeta.AddComponentMeta<MeshRendererMeta>());
			MeshRendererMeta_DeserializeFrom(meshRendererMeta, ss);
		}
		else if (ComponentType_SkinMeshRenderer == comType) {
			SkinMeshRendererMeta* skinMeshRendererMeta = static_cast<SkinMeshRendererMeta*>(prefabMeta.AddComponentMeta<SkinMeshRendererMeta>());
			SkinMeshRendererMeta_DeserializeFrom(skinMeshRendererMeta, ss);
		}
	}

	{
		string metaPath = path + FileSuffix::SUFFIX_META;
		FileHelper::ReadCharsFrom(metaPath, res);
		string str(res);
		stringstream ss(str);
		string line;
		while (getline(ss, line)) {
			istringstream iss(line);
			string key;
			if (!(iss >> key)) {
				break;
			}
			if (key == "guid:") {
				prefabMeta.guid = key.c_str();
				continue;
			}

			if (key == "name:") {
				prefabMeta.name = key.c_str();
				continue;
			}
		}
	}
}

void Serialization::SceneMeta_SerializeTo(const SceneMeta& sceneMeta, const string& path) {
	stringstream ss;

	ss << EditorDefaultConfig::DefaultAllGameObjectsStartStr() << endl;
	ss << endl;
	for (auto goMeta : sceneMeta.gameObjectMetas) {
		GameObjectMeta_SerializeTo(*goMeta, ss);
		ss << endl;
	}
	ss << EditorDefaultConfig::DefaultAllGameObjectsEndStr() << endl;
	ss << endl;

	ss << EditorDefaultConfig::DefaultAllPrefabInstancesStartStr() << endl;
	ss << endl;
	for (auto prefabInstanceMeta : sceneMeta.prefabInstanceMetas) {
		PrefabInstanceMeta_SerializeTo(*prefabInstanceMeta, ss);
		ss << endl;
	}
	ss << EditorDefaultConfig::DefaultAllPrefabInstancesEndStr() << endl;

	string result = ss.str();
	size_t len = result.length() + 1;
	char* charResult = new char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	delete[] charResult;
	cout << "Scene Serialize. " << endl;

	{
		stringstream ss;
		ss << "guid: " << sceneMeta.guid << endl;
		string result = ss.str();
		size_t len = result.length() + 1;
		char* charResult = new char[len];
		memcpy(charResult, result.c_str(), len);
		string metaPath = path + FileSuffix::SUFFIX_META;
		FileHelper::WriteCharsTo(metaPath, charResult);
		delete[] charResult;
		cout << "Scene Meta::Serialize | guid: " << sceneMeta.guid << endl;
	}
}

void Serialization::SceneMeta_DeserializeFrom(SceneMeta* sceneMeta, const string& path) {
	char* res = new char[FileHelper::GetFileCharSize(path)];
	FileHelper::ReadCharsFrom(path, res);
	string str(res);
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key))continue;

		if (key == EditorDefaultConfig::DefaultAllGameObjectsStartStr()) {
			while (getline(ss, line)) {
				iss = istringstream(line);

				if (!(iss >> key))continue;

				if (key == EditorDefaultConfig::DefaultAllGameObjectsEndStr()) {
					break;
				}

				if (key == EditorDefaultConfig::DefaultGameObjectStartStr()) {
					GameObjectMeta* goMeta = new GameObjectMeta();
					GameObjectMeta_DeserializeFrom(goMeta, ss);
					sceneMeta->gameObjectMetas.push_back(goMeta);
					continue;
				}
			}
		}
		else if (key == EditorDefaultConfig::DefaultAllPrefabInstancesStartStr()) {
			while (getline(ss, line)) {
				iss = istringstream(line);

				if (!(iss >> key))continue;
				if (key == EditorDefaultConfig::DefaultAllPrefabInstancesEndStr()) break;

				if (key == EditorDefaultConfig::DefaultPrefabInstanceStartStr()) {
					PrefabInstanceMeta* prefabInstanceMeta = new PrefabInstanceMeta();
					PrefabInstanceMeta_DeserializeFrom(prefabInstanceMeta, ss);
					sceneMeta->prefabInstanceMetas.push_back(prefabInstanceMeta);
					continue;
				}
			}
		}
	}

	delete[] res;

	{
		string metaPath = path + FileSuffix::SUFFIX_META;
		char* res = new char[FileHelper::GetFileCharSize(metaPath)];
		FileHelper::ReadCharsFrom(metaPath, res);
		string str(res);
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
		delete[] res;
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
	char* charResult = new char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);

	{
		stringstream ss;
		ss << "guid: " << materialMeta.guid << endl;
		string result = ss.str();
		size_t len = result.length() + 1;
		char* charResult = new char[len];
		memcpy(charResult, result.c_str(), len);
		string matMetaPath = path + FileSuffix::SUFFIX_META;
		FileHelper::WriteCharsTo(matMetaPath, charResult);
		cout << "Mat meta Serialize | guid: " << materialMeta.guid << endl;
	}
}

void Serialization::MaterialMeta_DeserializeFrom(MaterialMeta* materialMeta, const string& path) {
	unsigned int charCount = FileHelper::GetFileCharSize(path);
	if (charCount == 0) {
		cout << "############### no bytes. path: " << path << endl;
		return;
	}

	char* res = new char[charCount];
	FileHelper::ReadCharsFrom(path, res);
	string str(res);
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
			materialMeta->diffuseColor.x = atof(key.c_str());
			iss >> key;
			materialMeta->diffuseColor.y = atof(key.c_str());
			iss >> key;
			materialMeta->diffuseColor.z = atof(key.c_str());
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

		unsigned int charCount = FileHelper::GetFileCharSize(metaPath);
		if (charCount == 0) {
			cout << "############### no bytes. path: " << metaPath << endl;
			return;
		}

		char* res = new char[charCount];
		FileHelper::ReadCharsFrom(metaPath, res);
		string str(res);
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
		delete[] res;
	}
}

void Serialization::ShaderMeta_SerializeTo(const ShaderMeta& shaderMeta, const string& path) {
	stringstream ss;
	ss << "guid: " << shaderMeta.guid << endl;
	ss << endl;
	ss << "UniformsStart: " << endl;
	for (int i = 0; i < shaderMeta.uniforms.size(); i++) {
		ss << endl;
		ShaderUniform uniform = shaderMeta.uniforms[i];
		ss << "uniformName: " << uniform.name << endl;
		auto uniformType = uniform.type;
		ss << "uniformType: " << uniformType << endl;

		if (uniformType == ShaderUniformType_Int) {
			ss << "uniformValue: " << any_cast<int>(uniform.value) << endl;
		}
		else if (uniformType == ShaderUniformType_Float) {
			ss << "uniformValue: " << any_cast<float>(uniform.value) << endl;
		}
		else if (uniformType == ShaderUniformType_Float3) {
			vec3 v = any_cast<vec3>(uniform.value);
			ss << "uniformValue: " << v.x << ' ' << v.y << ' ' << v.z << endl;
		}
		else if (uniformType == ShaderUniformType_Float4) {
			vec4 v = any_cast<vec4>(uniform.value);
			ss << "uniformValue: " << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << endl;
		}
	}
	ss << endl;
	ss << "UniformsEnd" << endl;

	string result = ss.str();
	size_t len = result.length() + 1;
	char* charResult = new char[len];
	memcpy(charResult, result.c_str(), len);
	string shaderMetaPath = path + FileSuffix::SUFFIX_META;
	FileHelper::WriteCharsTo(shaderMetaPath, charResult);
	ss << "Shader Serialize | guid: " << shaderMeta.guid << endl;
}

void Serialization::ShaderMeta_DeserializeFrom(ShaderMeta* shaderMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;

	size_t charCount = FileHelper::GetFileCharSize(metaPath);
	if (charCount <= 0) {
		cout << "############### no bytes. path: " << metaPath << endl;
		return;
	}

	char* res = new  char[charCount];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(res);
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) continue;

		if (key == "guid:") {
			iss >> shaderMeta->guid;
		}
		else if (key == "UniformsStart:") {
			while (getline(ss, line)) {
				iss = istringstream(line);
				if (!(iss >> key)) continue;
				if (key == "UniformsEnd") break;

				ShaderUniform uniform = ShaderUniform();
				iss >> key;
				uniform.name = key;

				getline(ss, line);
				iss = istringstream(line);
				iss >> key;
				iss >> key;
				ShaderUniformType_ ut = (ShaderUniformType_)(atoi(key.c_str()));
				uniform.type = ut;

				if (ut == ShaderUniformType_Int) {
					getline(ss, line);
					iss = istringstream(line);
					iss >> key;
					iss >> key;

					int v = atoi(key.c_str());
					uniform.value = v;
				}
				else if (ut == ShaderUniformType_Float) {
					getline(ss, line);
					istringstream iss(line);
					iss >> key;
					iss >> key;

					float v = atoi(key.c_str());
					uniform.value = v;
				}
				else if (ut == ShaderUniformType_Float3) {
					getline(ss, line);
					iss = istringstream(line);
					iss >> key;
					iss >> key;

					vec3 v = vec3();
					v.x = atof(key.c_str());
					iss >> key;
					v.y = atof(key.c_str());
					iss >> key;
					v.z = atof(key.c_str());
					uniform.value = v;
				}
				else if (ut == ShaderUniformType_Float4) {
					getline(ss, line);
					iss = istringstream(line);
					iss >> key;
					iss >> key;

					vec4 v = vec4();
					v.x = atof(key.c_str());
					iss >> key;
					v.y = atof(key.c_str());
					iss >> key;
					v.z = atof(key.c_str());
					iss >> key;
					v.w = atof(key.c_str());
					uniform.value = v;
				}

				shaderMeta->uniforms.push_back(uniform);
			}
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
	char* charResult = new char[len];
	memcpy(charResult, result.c_str(), len);

	string metaPath = path + FileSuffix::SUFFIX_META;
	FileHelper::WriteCharsTo(metaPath, charResult);
}

void Serialization::TextureMeta_DeserializeFrom(TextureMeta* textureMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;

	unsigned int charCount = FileHelper::GetFileCharSize(metaPath);
	if (charCount == 0) {
		cout << "############### no bytes. path: " << metaPath << endl;
		return;
	}

	char* res = new char[charCount];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(res);
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
	char* charResult = new char[len];
	memcpy(charResult, result.c_str(), len);
	string objMetaPath = path + FileSuffix::SUFFIX_META;
	FileHelper::WriteCharsTo(objMetaPath, charResult);

	ss << "ObjMetadata::Serialize " << path << endl;
}

void Serialization::ObjMeta_DeserializeFrom(ObjMeta* objMeta, const string& path) {
	string metaPath = path + FileSuffix::SUFFIX_META;

	unsigned int charCount = FileHelper::GetFileCharSize(metaPath);
	if (charCount == 0) {
		cout << "############### no bytes. path: " << metaPath << endl;
		return;
	}

	char* res = new char[charCount];
	FileHelper::ReadCharsFrom(metaPath, res);
	string str(res);
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