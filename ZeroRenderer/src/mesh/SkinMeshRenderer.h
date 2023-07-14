#pragma once
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Component.h"
#include <vector>

using namespace std;

class SkinMeshRenderer :public Component {

public:
	vector<MeshFilter*>* meshFilters;
	vector<MeshRenderer*>* meshRenderers;
};

