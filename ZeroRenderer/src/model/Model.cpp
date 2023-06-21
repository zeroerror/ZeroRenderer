#include "Model.h"
#include <assimp/postprocess.h>

Model::Model() {
	transform = new Transform();
	allMeshes = new std::vector<Mesh*>();
	va = new VertexArray();
	vb = new VertexBuffer();
	ib = new IndexBuffer();

	vbLayout = VertexBufferLayout();
	vbLayout.Push<float>(3);
	vbLayout.Push<float>(2);
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

void Model::LoadModel(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "#################################ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
	BatchMeshes();
}

void Model::ProcessNode(aiNode* aNode, const aiScene* aScene) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		allMeshes->push_back(ProcessMesh(mesh, aScene));
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ProcessNode(childNode, aScene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* aMesh, const aiScene* aScene) {
	Mesh* mesh = new Mesh();
	std::vector<Vertex*>* vertices = mesh->vertices;

	aiVector3D* aTexCoords = aMesh->mTextureCoords[0];

	for (unsigned int i = 0; i < aMesh->mNumVertices; i++) {
		aiVector3D aPosition = aMesh->mVertices[i];
		aiVector3D aNormal = aMesh->mNormals[i];
		aiVector2D texCoord;
		if (aTexCoords != nullptr) {
			texCoord = aTexCoords[0][i];
		}

		Vertex* vertex = new Vertex();
		vertex->SetPosition(aPosition.x, aPosition.y, aPosition.z);
		vertex->SetNormal(aNormal.x, aNormal.y, aNormal.z);
		vertex->SetTexCoords(texCoord.x, texCoord.y);
		vertices->push_back(vertex);
	}

	std::vector<unsigned int>* indices = mesh->indices;
	for (unsigned int i = 0; i < aMesh->mNumFaces; i++) {
		aiFace face = aMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices->push_back(face.mIndices[j]);
		}
	}

	return mesh;
}

void Model::BatchMeshes() {
	std::vector<float> vertexData;
	std::vector<unsigned int> indiceArray;
	unsigned int vertexCount = 0;

	for (auto mesh : *allMeshes) {
		std::vector<Vertex*>* vertices = mesh->vertices;
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
		std::vector<unsigned int>* indices = mesh->indices;
		for (auto indice : *indices) {
			indiceArray.push_back(indice + vertexCount);
		}

		vertexCount += vertices->size();
	}

	va->Bind();
	vb->Ctor(vertexData.data(), vertexData.size());
	va->AddBuffer(vb, vbLayout);
	ib->Ctor(indiceArray.data(), indiceArray.size());

	std::cout << "Model BatchMeshes: Vertex float count: " << vertexData.size() << " Indice float count: " << indiceArray.size() << std::endl;
}

void Model::RenderModel() {
	va->Bind();
	ib->Bind();
	glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Model::RenderModelMesh() {
	std::vector<float> vertexData;
	std::vector<unsigned int> indiceArray;
	unsigned int vertexCount = 0;
	unsigned int indiceCount = 0;
	for (auto mesh : *allMeshes) {
		std::vector<Vertex*>* vertices = mesh->vertices;
		for (auto vertex : *vertices) {
			glm::vec3 position = vertex->position;
			glm::vec2 texCoords = vertex->texCoords;
			glm::vec3 normal = vertex->normal;
			vertexData.push_back(position.x);
			vertexData.push_back(position.y);
			vertexData.push_back(position.z);
			vertexCount++;
		}
		std::vector<unsigned int>* indices = mesh->indices;
		for (auto indice : *indices) {
			indiceArray.push_back(indice);
			indiceCount++;
		}

		VertexArray va = VertexArray();
		VertexBuffer vb = VertexBuffer();
		VertexBufferLayout vbLayout = VertexBufferLayout();
		IndexBuffer ib = IndexBuffer();
		va.Bind();
		vb.Ctor(vertexData.data(), vertexCount);
		vbLayout.Push<float>(3);
		va.AddBuffer(&vb, vbLayout);
		ib.Ctor(indiceArray.data(), indiceCount);
		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
