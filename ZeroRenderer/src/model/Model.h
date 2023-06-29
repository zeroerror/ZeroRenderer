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
	void Render();
	void RenderModelMesh(const Camera3D*& camera, const DirectLight*& light);

	Transform* transform;
	vector<Mesh*>* allMeshes;
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout vbLayout;
	IndexBuffer* ib;

private:
	void _BindShaderTemp(Shader* shader,const Camera3D*& camera,const DirectLight*& light) {
		glm::vec3 modelPos = transform->GetPosition();
		glm::quat modelRot = transform->GetRotation();
		glm::mat4 lightMVPMatrix = light->GetMVPMatrix_Perspective(modelPos);

		shader->Bind();
		shader->SetUniform1i("u_texture", 1);
		shader->SetUniformMat4f("u_mvp", lightMVPMatrix);
		shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(modelRot));
		shader->SetUniform3f("u_modPosition", modelPos.x, modelPos.y, modelPos.z);

		glm::vec3 lightPos = light->transform->GetPosition();
		glm::vec3 lightColor = light->color;
		glm::vec3 lightDirection = -light->GetLightDirection();
		shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
		shader->SetUniform3f("u_lightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
		shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);
		shader->SetUniformMat4f("u_lightMVPMatrix", lightMVPMatrix);
		shader->SetUniform1f("u_nearPlane", camera->nearPlane);
		shader->SetUniform1f("u_farPlane", camera->farPlane);
	}
};
