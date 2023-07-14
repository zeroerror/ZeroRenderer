#pragma once

#include <string>
#include <glm/gtx/quaternion.hpp>

#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "SkinMeshRenderer.h"

using namespace std;

// this is just a special gameobject,will remove this concept later.
class Model {

public:
	Transform* transform;
	SkinMeshRenderer* skinMeshRenderer;

	Material* material;

	Model();
	~Model();

	// remove to domain
	void BatchMeshes();

	// remove to domain
	void BatchedDraw();

	// this are runtime things,and this should be in mesh renderer
	bool isBatched;
	VertexArray* va_batched;
	VertexBuffer* vb_batched;
	VertexBufferLayout* vbLayout_batched;
	IndexBuffer* ib_batched;

};
