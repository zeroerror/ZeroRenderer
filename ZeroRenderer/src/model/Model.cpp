#include "Model.h"
#include <assimp/postprocess.h>
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

void Model::LoadModel(const string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "  #################################ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	string modelMetaPath = path + FileSuffix::SUFFIX_META;
	ObjMetadata objMeta = ObjMetadata();
	objMeta.DeserializeFrom(modelMetaPath);
	size_t materialIndex = 0;
	ProcessNode(scene->mRootNode, scene, objMeta, materialIndex);
}

void Model::ProcessNode(aiNode* aNode, const aiScene* aScene, const ObjMetadata& objMeta, size_t& materialIndex) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		allMeshes->push_back(ProcessMesh(mesh, aScene, objMeta, materialIndex));
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ProcessNode(childNode, aScene, objMeta, materialIndex);
	}
}

Mesh* Model::ProcessMesh(aiMesh* aiMesh, const aiScene* aScene, const ObjMetadata& objMeta, size_t& materialIndex) {
	Mesh* mesh = new Mesh();
	vector<Vertex*>* vertices = mesh->vertices;

	aiVector3D* aTexCoords = aiMesh->mTextureCoords[0];

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
		aiVector3D aPosition = aiMesh->mVertices[i];
		aiVector3D aNormal = aiMesh->mNormals[i];
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

	vector<unsigned int>* indices = mesh->indices;
	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
		aiFace face = aiMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices->push_back(face.mIndices[j]);
		}
	}

	string materialGUID = objMeta.materialGUIDs[materialIndex++];
	mesh->materialGUID = materialGUID;

	mesh->GenerateRenderer();

	return mesh;
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
