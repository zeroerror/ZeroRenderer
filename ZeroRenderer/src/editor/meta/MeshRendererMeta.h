#pragma once
#include <string>
#include "ComponentMeta.h"

using namespace std;

class MeshRendererMeta :public ComponentMeta {

public:
	string materialGUID;

	MeshRendererMeta() {
		componentType = ComponentType_MeshRenderer;
	}
};

