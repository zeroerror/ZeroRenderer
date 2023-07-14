#pragma once

#include <string>
#include "ComponentMeta.h"

using namespace std;

class MeshFilterMeta :public ComponentMeta {

public:
	string modelGUID;
	int meshIndex;

	MeshFilterMeta() {
		componentType = ComponentType_MeshFilter;
	}
};

