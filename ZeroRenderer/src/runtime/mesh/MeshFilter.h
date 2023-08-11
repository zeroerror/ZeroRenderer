#pragma once
#include "Mesh.h"
#include "Component.h"

class MeshFilter :public Component {

public:
	MeshFilter();
	~MeshFilter();

	Mesh* mesh;
};

