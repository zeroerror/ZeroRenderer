#pragma once

class Transform;
class Component;

#include <vector>
#include <sstream>
#include "Camera.h"
#include "DirectLight.h"

using namespace std;

class GameObject {

public:
	GameObject();
	~GameObject();

	string name;
	Transform* transform() { return _transform; };

	vector<Component*> GetAllComponents();

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	T* AddComponent() {
		T* component = new T();
		component->gameObject = this;
		component->transform = this->_transform;
		_components.push_back(component);
		return component;
	}

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	T* GetComponent() {
		T t = T();
		Component com = static_cast<Component>(t);
		ComponentType_ comType = com.componentType;
		for (auto com : _components) {
			if (comType == com->componentType) {
				return static_cast<T*>(com);
			}
		}

		return nullptr;
	}

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	void GetAllComponents(vector<T*>& components) {
		T t = T();
		Component com = static_cast<Component>(t);
		ComponentType_ comType = com.componentType;
		for (auto com : _components) {
			if (com->componentType == comType) {
				components.push_back(static_cast<T*>(com));
			}
		}
	}

private:
	Transform* _transform;
	vector<Component*> _components;

};
