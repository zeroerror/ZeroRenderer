#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

class Cube {

public:
	Cube();
	~Cube();
	void Ctor(float width, float height, float depth);

	static Cube* CreateCube(const float& width, const float& height, const float& depth);

public:
	Transform* transform;
	float width;
	float height;
	float depth;

	Texture* texture;
	Material* material;

	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;

private:
	VertexBufferLayout m_vbLayout;
	static unsigned int m_indiceArray[];
};

