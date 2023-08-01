#pragma once

class Transform;
class GameObject;

#include <sstream>
#include <string>
#include "ComponentType.h"

using namespace std;


class Component {
public:
	virtual ~Component() {}
	GameObject* gameObject;
	Transform* transform;
	ComponentType_ componentType;

private:
	string _name;

};
