#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

GameObject::GameObject() {
	transform = new Transform();
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
	transform->DeserializeFrom(ss);
	for (auto com : *_components) {
		com->DeserializeFrom(ss);
	}
}

void GameObject::AddComponent(Component* component) {
	_components->push_back(component);
}

vector<Component*>* _components;
