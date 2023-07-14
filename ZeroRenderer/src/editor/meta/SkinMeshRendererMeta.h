#pragma once
#include "MeshFilterMeta.h"
#include "MeshRendererMeta.h"
#include "ComponentMeta.h"
#include <vector>

using namespace std;

class SkinMeshRendererMeta :public ComponentMeta {

public:
	vector<MeshFilterMeta*> meshFilterMetas;
	vector<MeshRendererMeta*> meshRendererMetas;

	SkinMeshRendererMeta();
};

