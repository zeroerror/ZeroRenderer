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

	Transform* transform;

	void SerializeTo(stringstream& ss);
	void DeserializeFrom(stringstream& ss);

	template <typename T>
	Component* AddComponent() {
		Component* c = new T();
		c->gameObject = this;
		c->transform = this->transform;
		_components->push_back(c);
		return c;
	}

private:
	vector<Component*>* _components;

};

