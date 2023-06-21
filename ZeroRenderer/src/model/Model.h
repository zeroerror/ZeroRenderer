#pragma once

#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model {

public:
	Model();
	~Model();

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* aNode, const aiScene* aScene);
	Mesh* ProcessMesh(aiMesh* aMesh, const aiScene* aScene);
	void BatchMeshes();
	void RenderModel();
	void RenderModelMesh();

	Transform* transform;
	std::vector<Mesh*>* allMeshes;
	Texture* texture;
	Material* material;
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout vbLayout;
	IndexBuffer* ib;

};
