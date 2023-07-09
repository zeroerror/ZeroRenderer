#pragma once

class Transform;
class GameObject;

#include <sstream>

using namespace std;

enum ComponentType_ {
	ComponentType_Transform,
	ComponentType_Camera3D,
	ComponentType_DirectLight,
	ComponentType_SpotLight,
};

class Component {
public:
	virtual ~Component() {}
	GameObject* gameObject;
	Transform* transform;
	ComponentType_ componentType;
};
