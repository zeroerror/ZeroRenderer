#include "Serialization.h"
#include "FileHelper.h"

void Serialization::Transform_SerializeTo(Transform* transform, stringstream& ss) {
	ss << "componentType: " << transform->componentType << std::endl;
	ss << "position: " << transform->position.x << ' ' << transform->position.y << ' ' << transform->position.z << std::endl;
	ss << "rotation: " << transform->rotation.x << ' ' << transform->rotation.y << ' ' << transform->rotation.z << ' ' << transform->rotation.w << std::endl;
	ss << "componentEnd" << std::endl;
}

void Serialization::Transform_DeserializeFrom(Transform* transform, stringstream& ss) {
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
			transform->position.x = atoi(key.c_str());
			iss >> key;
			transform->position.y = atoi(key.c_str());
			iss >> key;
			transform->position.z = atoi(key.c_str());
		}
		else if (key == "rotation:") {
			iss >> key;
			transform->rotation.x = atoi(key.c_str());
			iss >> key;
			transform->rotation.y = atoi(key.c_str());
			iss >> key;
			transform->rotation.z = atoi(key.c_str());
			iss >> key;
			transform->rotation.w = atoi(key.c_str());
		}
	}
}

void Serialization::Camera_SerializeTo(Camera3D* camera, stringstream& ss) {
	ss << "componentType: " << camera->componentType << std::endl;
	ss << "scrWidth: " << camera->scrWidth << std::endl;
	ss << "scrHeight: " << camera->scrHeight << std::endl;
	ss << "fov: " << camera->fov << std::endl;
	ss << "nearPlane: " << camera->nearPlane << std::endl;
	ss << "farPlane: " << camera->farPlane << std::endl;
	ss << "orthoSize: " << camera->orthoSize << std::endl;
	ss << "componentEnd" << std::endl;
}

void Serialization::Camera_DeserializeFrom(Camera3D* camera, stringstream& ss) {
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
			camera->scrWidth = atof(key.c_str());
		}
		else if (key == "scrHeight:") {
			iss >> key;
			camera->scrHeight = atof(key.c_str());
		}
		else if (key == "fov:") {
			iss >> key;
			camera->fov = atof(key.c_str());
		}
		else if (key == "nearPlane:") {
			iss >> key;
			camera->nearPlane = atof(key.c_str());
		}
		else if (key == "farPlane:") {
			iss >> key;
			camera->farPlane = atof(key.c_str());
		}
		else if (key == "orthoSize:") {
			iss >> key;
			camera->orthoSize = atof(key.c_str());
		}
	}
}

void Serialization::Component_SerializeTo(Component* component, stringstream& ss) {
	if (component->componentType == ComponentType_Transform)Camera_SerializeTo(dynamic_cast<Camera3D*>(component), ss);
	if (component->componentType == ComponentType_Camera3D)Camera_SerializeTo(dynamic_cast<Camera3D*>(component), ss);
}

void Serialization::GameObject_SerializeTo(GameObject* gameObject, stringstream& ss) {
	Transform_SerializeTo(gameObject->transform(), ss);
	vector<Component*> components = gameObject->GetAllComponents();
	for (auto com : components) {
		Component_SerializeTo(com, ss);
	}
}

void Serialization::GameObject_DeserializeFrom(GameObject* gameObject, stringstream& ss) {
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
			Transform* transform = gameObject->transform();
			Transform_DeserializeFrom(transform, ss);
		}
		else if (ComponentType_Camera3D == (ComponentType_)atoi(key.c_str())) {
			Camera3D* camera = dynamic_cast<Camera3D*>(gameObject->AddComponent<Camera3D>());
			Camera_DeserializeFrom(camera, ss);
			camera->gameObject = gameObject;
			camera->transform = gameObject->transform();
		}
	}
}

void Serialization::Scene_SerializeTo(Scene* scene, const string& path) {
	stringstream ss;
	ss << "GameObjects: " << endl;
	for (auto go : scene->gameObjects) {
		ss << "GameObject:" << endl;
		GameObject_SerializeTo(go, ss);
	}
	ss << "---" << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	delete charResult;
}

void Serialization::Scene_DeserializeFrom(Scene* scene, const string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
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
					GameObject* go = new GameObject();
					GameObject_DeserializeFrom(go, ss);
					scene->gameObjects.push_back(go);
				}
				else if (key == "---") {
					break;
				}
			}
		}
	}
	delete res;
}

void Serialization::MatMeta_SerializeTo(MatMeta* matMeta, const std::string& path) {
	std::stringstream ss;
	ss << "guid: " << matMeta->guid << std::endl;
	std::string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);

	ss << "Material Meta::Serialize | guid: " << matMeta->guid << std::endl;
}

void Serialization::MatMeta_DeserializeFrom(MatMeta* matMeta, const std::string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
	std::string str(reinterpret_cast<char*>(res));
	std::stringstream ss(str);
	std::string line;
	while (std::getline(ss, line)) {
		std::istringstream iss(line);
		std::string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> matMeta->guid;
		}
	}
}

void Serialization::ShaderMeta_SerializeTo(ShaderMeta* shaderMeta, const string& path) {
	std::stringstream ss;
	ss << "guid: " << shaderMeta->guid << std::endl;
	ss << "useLightingMVP: " << shaderMeta->useLightingMVP << std::endl;
	std::string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	ss << "Shader Serialize | guid: " << shaderMeta->guid << std::endl;
}

void Serialization::ShaderMeta_DeserializeFrom(ShaderMeta* shaderMeta, const string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
	std::string str(reinterpret_cast<char*>(res));
	std::stringstream ss(str);
	std::string line;
	while (std::getline(ss, line)) {
		std::istringstream iss(line);
		std::string key;
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
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
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
	std::stringstream ss;
	ss << "guid: " << objMeta.guid << std::endl;
	ss << "materials: {" << std::endl;
	for (auto matGUID : objMeta.materialGUIDs) {
		ss << "\t" << matGUID << std::endl;
	}
	ss << "}" << std::endl;
	ss << "meshNames: {" << std::endl;
	for (auto meshName : objMeta.meshNames) {
		ss << "\t" << meshName << std::endl;
	}
	ss << "}" << std::endl;

	std::string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);

	ss << "ObjMetadata::Serialize | guid: " << objMeta.guid << std::endl;
}

void Serialization::ObjMeta_DeserializeFrom(ObjMeta* objMeta, const string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
	std::string str(reinterpret_cast<char*>(res));
	std::stringstream ss(str);
	std::string line;
	while (getline(ss, line)) {
		std::istringstream iss(line);
		std::string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> objMeta->guid;
		}
		else if (key == "materials:") {
			while (std::getline(ss, line)) {
				std::istringstream iss2(line);
				string materialGUID;
				if (!(iss2 >> materialGUID) || materialGUID == "}") {
					break;
				}
				objMeta->materialGUIDs.push_back(materialGUID);
			}
		}
		else if (key == "meshNames:") {
			while (std::getline(ss, line)) {
				std::istringstream iss2(line);
				string meshName;
				if (!(iss2 >> meshName) || meshName == "}") {
					break;
				}
				objMeta->meshNames.push_back(meshName);
			}
		}
	}
}

