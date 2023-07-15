#pragma once

class Transform;
class GameObject;

#include <sstream>
#include <string>

using namespace std;

enum ComponentType_ :int {
	ComponentType_None,
	ComponentType_Transform,
	ComponentType_Camera,
	ComponentType_MeshFilter,
	ComponentType_MeshRenderer,
	ComponentType_SkinMeshRenderer,
	ComponentType_DirectLight,
	ComponentType_SpotLight,
};

static const char* ComponentType_Names[] = {
	"ComponentType_None",
	"ComponentType_Transform",
	"ComponentType_Camera",
	"ComponentType_MeshFilter",
	"ComponentType_MeshRenderer",
	"ComponentType_SkinMeshRenderer",
	"ComponentType_DirectLight",
	"ComponentType_SpotLight"
};

static const ComponentType_ GetComponentType(const string& name) {
	for (int i = 0; i < 7; i++) {
		if (name == ComponentType_Names[i]) {
			return (ComponentType_)i;
		}
	}

	return ComponentType_None;
}

class Component {
public:
	virtual ~Component() {}
	GameObject* gameObject;
	Transform* transform;
	ComponentType_ componentType;
};
