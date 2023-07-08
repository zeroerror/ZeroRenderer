#pragma once

class Transform;
class Component;
#include "Transform.h"
#include "Component.h"

#include <vector>
#include <sstream>

using namespace std;

class GameObject {

public:
	GameObject();
	~GameObject();

	Transform* transform;

	void SerializeTo(stringstream& ss);
	void DeserializeFrom(stringstream& ss);
	void AddComponent(Component* component);

	template <typename T>
	void RemoveComponent() {
		for (auto it = _components->begin(); it != _components->end(); ++it) {
			T* component = dynamic_cast<T*>(*it);
			if (component != nullptr) {
				_components->erase(it);
				delete component;
				break; 
			}
		}
	}

private:
	vector<Component*>* _components;

};

