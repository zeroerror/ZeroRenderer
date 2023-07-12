#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "MeshFilter.h"

using namespace std;

class MeshRenderer {

public:
	string materialGUID;

	Material* material;
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout* vbLayout;
	IndexBuffer* ib;

	MeshRenderer();
	~MeshRenderer();

	void GenerateRenderer(const MeshFilter* meshFilter);
};

