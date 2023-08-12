#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Component.h"
#include "MeshFilter.h"

using namespace std;

class MeshRenderer :public Component {

public:
	string materialGUID;

	// gl
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout* vbLayout;
	IndexBuffer* ib;
	void GenerateRenderer(const MeshFilter* meshFilter);

	MeshRenderer();
	~MeshRenderer();

};

