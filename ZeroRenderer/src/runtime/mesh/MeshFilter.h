#pragma once
#include "Mesh.h"
#include "Component.h"
#include <string>

using namespace glm;

class MeshFilter : public Component
{

public:
	Mesh *mesh;

	// Editor Cache
	string modelGUID_editorCache;
	int meshIndex_editorCache;

	MeshFilter();
	~MeshFilter();
};
