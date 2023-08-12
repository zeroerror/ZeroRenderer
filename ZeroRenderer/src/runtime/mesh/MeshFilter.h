#pragma once
#include "Mesh.h"
#include "Component.h"

using namespace glm;

class MeshFilter :public Component {

public:
	Mesh* mesh;
	
	MeshFilter();
	~MeshFilter();

};

