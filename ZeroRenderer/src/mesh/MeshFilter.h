#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

using namespace std;

class MeshFilter {

public:
	string materialGUID;

	std::vector<Vertex*>* vertices;
    std::vector<unsigned int>* indices;

	MeshFilter();
	~MeshFilter();
	
};

