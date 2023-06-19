#include "Model.h"

Model::Model() {
	transform = new Transform();
	allMeshes = new std::vector<Mesh*>();
	va = new VertexArray();
	vb = new VertexBuffer();
	ib = new IndexBuffer();

	vbLayout = VertexBufferLayout();
    vbLayout.Push<float>(3);
    vbLayout.Push<float>(2);
    vbLayout.Push<float>(3);
}

Model::~Model() {
    delete transform;
    // Consider Reuse
    delete allMeshes;
    delete va;
    delete vb;
    delete ib;
    // Consider Reuse
}

void Model::BatchMeshes() {
    unsigned int vertexCount = 0;
    std::vector<float> vertexData;

    unsigned int indiceCount = 0;
    std::vector<unsigned int> indiceArray;

    for (auto mesh : *allMeshes) {
        std::vector<Vertex*>* vertices = mesh->vertices;
        for (auto vertex : *vertices) {
            glm::vec3 position = vertex->position;
            glm::vec2 texCoords = vertex->texCoords;
            glm::vec3 normal = vertex->normal;

            // 将顶点数据添加到向量中
            vertexData.push_back(position.x);
            vertexData.push_back(position.y);
            vertexData.push_back(position.z);
            vertexData.push_back(texCoords.x);
            vertexData.push_back(texCoords.y);
            vertexData.push_back(normal.x);
            vertexData.push_back(normal.y);
            vertexData.push_back(normal.z);

            vertexCount++;
        }

        std::vector<unsigned int>* indices = mesh->indices;
        for (auto indice : *indices) {
            indiceArray.push_back(indice);
            indiceCount++;
        }
    }

    va->Bind();
    vb->Ctor(vertexData.data(), vertexCount * sizeof(float));  
    va->AddBuffer(vb, vbLayout);
	ib->Ctor(indiceArray.data(), indiceCount * sizeof(unsigned int));
}
