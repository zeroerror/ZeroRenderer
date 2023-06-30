#include "Model.h"
#include "Database.h"
#include "FileSuffix.h"

Model::Model() {
	transform = new Transform();
	allMeshes = new vector<Mesh*>();

	va_batched = new VertexArray();
	vb_batched = new VertexBuffer();
	vbLayout_batched = new VertexBufferLayout();
	vbLayout_batched->Push<float>(3);
	vbLayout_batched->Push<float>(2);
	ib_batched = new IndexBuffer();
}

Model::~Model() {
	delete transform;
	// Consider Reuse
	delete allMeshes;
	delete va_batched;
	delete vb_batched;
	delete vbLayout_batched;
	delete ib_batched;
	// Consider Reuse
}

void Model::BatchMeshes() {
	vector<float> vertexData;
	vector<unsigned int> indiceArray;
	unsigned int vertexCount = 0;

	for (auto mesh : *allMeshes) {
		vector<Vertex*>* vertices = mesh->vertices;
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
		vector<unsigned int>* indices = mesh->indices;
		for (auto indice : *indices) {
			indiceArray.push_back(indice + vertexCount);
		}

		vertexCount += vertices->size();
	}

	va_batched->Bind();
	vb_batched->Ctor(vertexData.data(), vertexData.size());
	va_batched->AddBuffer(vb_batched, vbLayout_batched);
	ib_batched->Ctor(indiceArray.data(), indiceArray.size());

	isBatched = true;
	cout << "Model BatchMeshes: Vertex float count: " << vertexData.size() << " Indice float count: " << indiceArray.size() << endl;
}

void Model::BatchedDraw() {
	va_batched->Bind();
	ib_batched->Bind();
	glDrawElements(GL_TRIANGLES, ib_batched->GetCount(), GL_UNSIGNED_INT, nullptr);
}
