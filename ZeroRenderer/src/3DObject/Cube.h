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

	IndexBuffer* GetIndexBuffer();
	static Cube* CreateCube(const float& width, const float& height, const float& depth);

public:
	Transform transform;
	Texture* texture;
	Material* material;
	VertexArray va;
	float width;
	float height;
	float depth;
private:
	VertexBuffer m_vb;
	VertexBufferLayout m_vbLayout;
	static unsigned int m_indiceArray[];
	static IndexBuffer m_ib;
	static bool m_ibInit;
};

