#pragma once
#include <vector>

#include "Vertex.h"

class Mesh {

public:
    std::vector<Vertex*>* vertices;
    std::vector<unsigned int>* indices;

	Mesh();
	~Mesh();
};

