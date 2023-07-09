#pragma once

class Transform;
class Component;

#include <vector>
#include <sstream>

using namespace std;

class GameObject {

public:
	GameObject();
	~GameObject();

	Transform* transform() { return _transform; };
	vector<Component*> GetAllComponents();

	template <typename T>
	Component* AddComponent() {
		Component* c = new T();
		c->gameObject = this;
		c->transform = this->_transform;
		_components.push_back(c);
		return c;
	}

private:
	Transform* _transform;
	vector<Component*> _components;

};

