#include "MeshRenderer.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {
	va = new VertexArray();
	vb = new VertexBuffer();
	ib = new IndexBuffer();
	vbLayout = new VertexBufferLayout();
	vbLayout->Push<float>(3);
	vbLayout->Push<float>(2);
	vbLayout->Push<float>(3);
}

MeshRenderer::~MeshRenderer() {
	delete va;
	delete vb;
	delete ib;
	delete vbLayout;
}

void MeshRenderer::GenerateRenderer(const MeshFilter* meshFilter) {
	vector<float> vertexData = vector<float>();
	vector<unsigned int> indiceArray = vector<unsigned int>();

	vector<Vertex*>* vertices = meshFilter->mesh->vertices;
	vector<unsigned int>* indices = meshFilter->mesh->indices;
	for (auto vertex : *vertices) {
		glm::vec3 position = vertex->position;
		glm::vec2 texCoords = vertex->texCoords;
		glm::vec3 normal = vertex->normal;
		vertexData.push_back(position.x);
		vertexData.push_back(position.y);
		vertexData.push_back(position.z);
		vertexData.push_back(texCoords.x);
		vertexData.push_back(texCoords.y);
		vertexData.push_back(normal.x);
		vertexData.push_back(normal.y);
		vertexData.push_back(normal.z);
	}
	for (auto indice : *indices) {
		indiceArray.push_back(indice);
	}

	va->Bind();
	vb->Ctor(vertexData.data(), vertexData.size());
	va->AddBuffer(vb, vbLayout);
	ib->Ctor(indiceArray.data(), indiceArray.size());
}
