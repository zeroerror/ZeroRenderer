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
	void AddComponent(Component* component);

private:
	vector<Component*>* _components;

};

