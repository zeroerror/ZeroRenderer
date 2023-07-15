#pragma once
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Component.h"
#include <vector>

using namespace std;

class SkinMeshRenderer :public Component {

public:
	vector<MeshFilter*>* meshFilters;
	vector<MeshRenderer*>* meshRenderers;

	// this are runtime things,and this should be in mesh renderer
	bool isBatched;
	VertexArray* va_batched;
	VertexBuffer* vb_batched;
	VertexBufferLayout* vbLayout_batched;
	IndexBuffer* ib_batched;

};

