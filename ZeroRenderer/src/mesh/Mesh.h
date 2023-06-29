#pragma once
#include <string>
#include <vector>
#include "Vertex.h"

using namespace std;

class Mesh {

public:
    std::vector<Vertex*>* vertices;
    std::vector<unsigned int>* indices;
    string materialGUID;

	Mesh();
	~Mesh();
};

