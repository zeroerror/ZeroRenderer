#pragma once
#include "Vertex.h"
#include <string>
#include <vector>
using namespace std;

class Mesh {

public:
	string meshName;
	vector<Vertex*>* vertices;
	vector<unsigned int>* indices;

	Mesh();
	~Mesh();
};

