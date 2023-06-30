#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

class Rectangle {

public:
	Rectangle();
	~Rectangle();
	void Ctor(float width, float height);

	static Rectangle* CreateRectangle(const float& width, const float& height );

public:
	Transform* transform;
	float width;
	float height;

	Texture* texture;
	Material* material;

	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;

private:
	VertexBufferLayout* m_vbLayout;
	static unsigned int m_indiceArray[];
};

