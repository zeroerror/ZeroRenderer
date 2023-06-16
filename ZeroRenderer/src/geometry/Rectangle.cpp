#include "Rectangle.h"
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Material.h"

Rectangle::Rectangle() {
	std::cout << "Rectangle::Rectangle()" << std::endl;
	transform = new Transform();
}

void Rectangle::Ctor(float width, float height) {
	this->width = width;
	this->height = height;

	this->va = new VertexArray();
	this->va->Ctor();

	this->vb = new VertexBuffer();
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	this->vb->Ctor(new float[16]{
		// 顶点坐标 + 纹理坐标
		-halfWidth, -halfHeight, 0.0f, 0.0f,
		halfWidth, -halfHeight, 1.0f, 0.0f,
		halfWidth, halfHeight, 1.0f, 1.0f,
		-halfWidth, halfHeight, 0.0f, 1.0f
				   }, 16 * sizeof(float));

	this->m_vbLayout = VertexBufferLayout();
	this->m_vbLayout.Push<float>(2);
	this->m_vbLayout.Push<float>(2);

	this->va->AddBuffer(vb, m_vbLayout);

	this->ib = new IndexBuffer();
	this->ib->Ctor(m_indiceArray, 36);
}

Rectangle::~Rectangle() {
	std::cout << "Rectangle::~Rectangle()" << std::endl;
	delete vb;
	delete va;
	delete ib;
}

Rectangle* Rectangle::CreateRectangle(const float& width, const float& height) {
	Rectangle* cube = new Rectangle();
	cube->Ctor(width, height);
	return cube;
}

unsigned int Rectangle::m_indiceArray[36] = {
	0, 1, 2,  // 面0
	2, 3, 0,
};

