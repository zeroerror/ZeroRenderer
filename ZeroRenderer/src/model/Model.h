#pragma once

#include <string>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjMetadata.h"
#include "Camera3D.h"
#include "DirectLight.h"

using namespace std;

class Model {

public:
	Model();
	~Model();

	void LoadModel(const string& path);
	void ProcessNode(aiNode* aiNode, const aiScene* aiScene, const ObjMetadata& objMeta, size_t& materialIndex);
	Mesh* ProcessMesh(aiMesh* aMesh, const aiScene* aiScene, const ObjMetadata& objMeta, size_t& materialIndex);
	void BatchMeshes();
	void BatchedDraw();

	Transform* transform;
	vector<Mesh*>* allMeshes;

	VertexArray* va_batched;
	VertexBuffer* vb_batched;
	VertexBufferLayout* vbLayout_batched;
	IndexBuffer* ib_batched;

	Material* material;

	bool isBatched;

};
