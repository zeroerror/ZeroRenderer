#include "Serialization.h"

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
