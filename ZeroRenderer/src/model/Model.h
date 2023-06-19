#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"

class Model {

public:
	Model();
	~Model();

	void BatchMeshes();

	Transform* transform;
	std::vector<Mesh*>* allMeshes;
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout vbLayout;
	IndexBuffer* ib;

	Texture* texture;
	Material* material;
};
