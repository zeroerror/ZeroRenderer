#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "Camera3D.h"

GameObject::GameObject() {
	transform = new Transform();
	transform->gameObject = this;
	_components = new vector<Component*>();
}

GameObject::~GameObject() {

}

void GameObject::SerializeTo(stringstream& ss) {
	transform->SerializeTo(ss);
	for (auto com : *_components) {
		com->SerializeTo(ss);
	}
}

void GameObject::DeserializeFrom(stringstream& ss) {
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
			transform->DeserializeFrom(ss);
		}
		else if (ComponentType_Camera3D == (ComponentType_)atoi(key.c_str())) {
			Camera3D* camera = new Camera3D();
			camera->DeserializeFrom(ss);
			camera->gameObject = this;
			camera->transform = this->transform;
			_components->push_back(camera);
		}
	}
}

vector<Component*>* _components;
