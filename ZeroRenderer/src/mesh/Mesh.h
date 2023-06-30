#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

using namespace std;

class Mesh {

public:
	string materialGUID;

	std::vector<Vertex*>* vertices;
    std::vector<unsigned int>* indices;
	Material* material;
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout* vbLayout;
	IndexBuffer* ib;

	Mesh();
	~Mesh();

	void GenerateRenderer();
};

