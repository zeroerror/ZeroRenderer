#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

using namespace std;

class Mesh {

public:
    std::vector<Vertex*>* vertices;
    std::vector<unsigned int>* indices;
    string materialGUID;

	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout* vbLayout;
	IndexBuffer* ib;

	Mesh();
	~Mesh();

	void GenerateRenderer();
};

