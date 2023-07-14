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
		_components.push_back(component);
		return component;
	}

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	T* GetComponent() {
		for (auto com : _components) {
			if (typeid(com) == typeid(T)) {
				return static_cast<T*>(com);
			}
		}

		return nullptr;
	}

private:
	Transform* _transform;
	vector<Component*> _components;

};

