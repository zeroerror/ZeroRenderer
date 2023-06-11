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
	Transform transform;
	Texture* texture;
	Material* material;
	VertexArray* va;
	static IndexBuffer* ib;
	float width;
	float height;
	float depth;
private:
	VertexBuffer m_vb;
	VertexBufferLayout m_vbLayout;
	static unsigned int m_indiceArray[];
	static bool m_ibInit;
};

