#pragma once
#include "MeshFilter.h"
#include "MeshRenderer.h"

using namespace std;

class Mesh {

public:
    MeshFilter *meshFilter;
    MeshRenderer *meshRenderer;

	Mesh();
	~Mesh();
	void GenerateRenderer();
};

