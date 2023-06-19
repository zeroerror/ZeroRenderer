#include "Cube.h"
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Material.h"

Cube::Cube() {
	std::cout << "Cube::Cube()" << std::endl;
	transform = new Transform();
}

void Cube::Ctor(float width, float height, float depth) {
	this->width = width;
	this->height = height;
	this->depth = depth;

	this->va = new VertexArray();

	this->vb = new VertexBuffer();
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;
	float plane_fb = width * height;
	float plane_lr = height * depth;
	float plane_ud = width * depth;
	glm::vec3 normal_f = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 normal_b = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 normal_l = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 normal_r = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 normal_u = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 normal_d = glm::vec3(0.0f, -1.0f, 0.0f);
	float planeTotal = plane_fb + plane_lr + plane_ud;
	float ratio_fb = plane_fb / planeTotal;
	float ratio_lr = plane_lr / planeTotal;
	float ratio_ud = plane_ud / planeTotal;
	glm::vec3 normal_ldb = normal_l * ratio_lr + normal_d * ratio_ud + normal_b * ratio_fb;
	normal_ldb = glm::normalize(normal_ldb);
	glm::vec3 normal_rdb = normal_r * ratio_lr + normal_d * ratio_ud + normal_b * ratio_fb;
	normal_rdb = glm::normalize(normal_rdb);
	glm::vec3 normal_rub = normal_r * ratio_lr + normal_u * ratio_ud + normal_b * ratio_fb;
	normal_rub = glm::normalize(normal_rub);
	glm::vec3 normal_lub = normal_l * ratio_lr + normal_u * ratio_ud + normal_b * ratio_fb;
	normal_lub = glm::normalize(normal_lub);
	glm::vec3 normal_ldf = normal_l * ratio_lr + normal_d * ratio_ud + normal_f * ratio_fb;
	normal_ldf = glm::normalize(normal_ldf);
	glm::vec3 normal_rdf = normal_r * ratio_lr + normal_d * ratio_ud + normal_f * ratio_fb;
	normal_rdf = glm::normalize(normal_rdf);
	glm::vec3 normal_ruf = normal_r * ratio_lr + normal_u * ratio_ud + normal_f * ratio_fb;
	normal_ruf = glm::normalize(normal_ruf);
	glm::vec3 normal_luf = normal_l * ratio_lr + normal_u * ratio_ud + normal_f * ratio_fb;
	normal_luf = glm::normalize(normal_luf);

	this->vb->Ctor(new float[64]{
		// 顶点坐标 + 纹理坐标
		-halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f, normal_ldb.x,normal_ldb.y,normal_ldb.z,
		halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f, normal_rdb.x,normal_rdb.y,normal_rdb.z,
		halfWidth, halfHeight, -halfDepth, 1.0f, 1.0f, normal_rub.x,normal_rub.y,normal_rub.z,
		-halfWidth, halfHeight, -halfDepth, 0.0f, 1.0f, normal_lub.x,normal_lub.y,normal_lub.z,
		-halfWidth, -halfHeight, halfDepth, 0.0f, 0.0f, normal_ldf.x,normal_ldf.y,normal_ldf.z,
		halfWidth, -halfHeight, halfDepth, 1.0f, 0.0f, normal_rdf.x,normal_rdf.y,normal_rdf.z,
		halfWidth, halfHeight, halfDepth, 1.0f, 1.0f, normal_ruf.x,normal_ruf.y,normal_ruf.z,
		-halfWidth, halfHeight, halfDepth, 0.0f, 1.0f, normal_luf.x,normal_luf.y,normal_luf.z
				   }, 64 * sizeof(float));

	this->m_vbLayout = VertexBufferLayout();
	this->m_vbLayout.Push<float>(3);
	this->m_vbLayout.Push<float>(2);
	this->m_vbLayout.Push<float>(3);

	this->va->AddBuffer(vb, m_vbLayout);

	this->ib = new IndexBuffer();
	this->ib->Ctor(m_indiceArray, 36);
}

Cube::~Cube() {
	std::cout << "Cube::~Cube()" << std::endl;
	delete transform;
	delete va;
	delete vb;
	delete ib;
}

Cube* Cube::CreateCube(const float& width, const float& height, const float& depth) {
	Cube* cube = new Cube();
	cube->Ctor(width, height, depth);
	return cube;
}

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

