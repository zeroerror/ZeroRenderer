#include "Mesh.h"

Mesh::Mesh() {
	vertices = new std::vector<Vertex*>();
	indices = new std::vector<unsigned int>();

	va = new VertexArray();
	vb = new VertexBuffer();
	ib = new IndexBuffer();
	vbLayout = new VertexBufferLayout();
	vbLayout->Push<float>(3);
	vbLayout->Push<float>(2);
}

void Mesh::GenerateRenderer(){
	vector<float> vertexData;
	vector<unsigned int> indiceArray;

	for (auto vertex : *vertices) {
		glm::vec3 position = vertex->position;
		glm::vec2 texCoords = vertex->texCoords;
		glm::vec3 normal = vertex->normal;
		vertexData.push_back(position.x);
		vertexData.push_back(position.y);
		vertexData.push_back(position.z);
		vertexData.push_back(texCoords.x);
		vertexData.push_back(texCoords.y);
	}
	for (auto indice : *indices) {
		indiceArray.push_back(indice);
	}

	va->Bind();
	vb->Ctor(vertexData.data(), vertexData.size());
	va->AddBuffer(vb, vbLayout);
	ib->Ctor(indiceArray.data(), indiceArray.size());
}
