#include "Cube.h"
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Material.h"

Cube::Cube() {}

void Cube::Ctor(float width, float height, float depth) {
	this->width = width;
	this->height = height;
	this->depth = depth;
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	this->va = new VertexArray();
	this->va->Ctor();

	this->m_vb = VertexBuffer();
	this->m_vb.Ctor(new float[40]{
		// 顶点坐标 + 纹理坐标
		-halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f,     // 顶点0
		halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f,      // 顶点1
		halfWidth, halfHeight, -halfDepth, 1.0f, 1.0f,       // 顶点2
		-halfWidth, halfHeight, -halfDepth, 0.0f, 1.0f,      // 顶点3
		-halfWidth, -halfHeight, halfDepth, 0.0f, 0.0f,      // 顶点4
		halfWidth, -halfHeight, halfDepth, 1.0f, 0.0f,       // 顶点5
		halfWidth, halfHeight, halfDepth, 1.0f, 1.0f,        // 顶点6
		-halfWidth, halfHeight, halfDepth, 0.0f, 1.0f        // 顶点7
		}, 40 * sizeof(float));

	this->m_vbLayout = VertexBufferLayout();
	this->m_vbLayout.Push<float>(3);
	this->m_vbLayout.Push<float>(2);

	this->va->AddBuffer(m_vb, m_vbLayout);

	if (!this->m_ibInit) {
		this->ib = new IndexBuffer();
		this->ib->Ctor(m_indiceArray, 36);
		this->m_ibInit = true;
		std::cout << "Init InderBuffer" << std::endl;
	}
}

Cube::~Cube() {
	std::cout << "Cube::~Cube()" << std::endl;
}

Cube* Cube::CreateCube(const float& width, const float& height, const float& depth) {
	Cube* cube = new Cube();
	cube->Ctor(width, height, depth);
	return cube;
}

IndexBuffer* Cube::ib;
bool Cube::m_ibInit;

unsigned int Cube::m_indiceArray[36] = {
	0, 1, 2,  // 面0
	2, 3, 0,
	1, 5, 6,  // 面1
	6, 2, 1,
	5, 4, 7,  // 面2
	7, 6, 5,
	4, 0, 3,  // 面3
	3, 7, 4,
	3, 2, 6,  // 面4
	6, 7, 3,
	4, 5, 1,  // 面5
	1, 0, 4
};

