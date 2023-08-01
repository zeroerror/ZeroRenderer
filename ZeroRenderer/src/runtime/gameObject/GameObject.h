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

	void SetName(const string& name);
	string GetName() const;
	Transform* transform() const;
	vector<Component*> GetAllComponents();
	GameObject* Find(const string& path);
	int GetGID();

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
		if (comType == ComponentType_Transform) {
			return dynamic_cast<T*>(_transform);
		}

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
		Component c = static_cast<Component>(t);
		ComponentType_ comType = c.componentType;
		if (comType == ComponentType_Transform) {
			components.push_back(dynamic_cast<T*>(_transform));
		}

		for (auto com : _components) {
			if (com->componentType == comType) {
				components.push_back(dynamic_cast<T*>(com));
			}
		}
	}

private:
	string _name;
	int _gid;
	Transform* _transform;
	vector<Component*> _components;
	GameObject* _Find(const string& path, GameObject* gameObject) const;
	GameObject* _Find(const string& name) const;

};

