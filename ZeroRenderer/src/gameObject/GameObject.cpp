#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "Camera3D.h"

GameObject::GameObject() {
	_transform = new Transform();
	_transform->gameObject = this;
	_components = vector<Component*>();
}

GameObject::~GameObject() {
}

vector<Component*> GameObject::GetAllComponents() {
	return _components;
}

vector<Component*>* _components;
