#include "EditorRendererDomain.h"
#include "GLDebug.h"
#include "EditorDatabase.h"
#include "ShaderMeta.h"
#include "FileSuffix.h"
#include "FileHelper.h"
#include "Serialization.h"
#include "SkinMeshRenderer.h"
#include "SkinMeshRendererMeta.h"
#include "EditorModelManager.h"

#include <assimp/postprocess.h>
#include <filesystem>
#include <glm/gtx/quaternion.hpp>
#include <src/vendor/glm/gtx/string_cast.hpp>

using namespace Serialization;
using namespace std;
using namespace glm;
using namespace filesystem;
namespace fs = filesystem;

void EditorRendererDomain::Inject(EditorContext* ctxt) {
	this->editorContext = ctxt;
}

void EditorRendererDomain::Init() {
	// Load all models'meshes to mesh repo.
	const size_t suffixCount = 1;
	string suffixes[suffixCount] = { FileSuffix::SUFFIX_OBJ };
	unsigned int suffixFlag = FileSuffix::ToFileSuffixFlag(suffixes, suffixCount);

	vector<string> filePaths;
	FileHelper::GetFilePaths("asset", suffixFlag, filePaths);
	for (auto path : filePaths) {
		ProcessModel(path);
	}

	// Generate default scene.
	EditorDatabase::GenerateDefaultSceneMeta();
}

void EditorRendererDomain::ProcessModel(const string& path) {
	const aiScene* aScene = nullptr;
	if (!EditorModelManager::TryLoadModel(path, aScene)) {
		return;
	}

	PrefabMeta prefabMeta = PrefabMeta();
	PrefabMeta_DeserializeFrom(&prefabMeta, path);
	ProcessMeshes(aScene, prefabMeta);
}

void EditorRendererDomain::ProcessMeshes(const aiScene* aScene, PrefabMeta& prefabMeta) {
	MeshRepo* meshRepo = editorContext->GetMeshRepo();
	SkinMeshRendererMeta* skinMeshRendererMeta = prefabMeta.GetComponentMeta<SkinMeshRendererMeta>();
	for (size_t i = 0; i < skinMeshRendererMeta->meshFilterMetas.size(); i++) {
		MeshFilterMeta* meshFilterMeta = skinMeshRendererMeta->meshFilterMetas[i];
		string modelGUID = meshFilterMeta->modelGUID;
		int meshIndex = meshFilterMeta->meshIndex;
		Mesh* mesh = new Mesh();
		aiMesh* aMesh = aScene->mMeshes[meshIndex];
		mesh->meshName = aMesh->mName.C_Str();
		meshRepo->TryAddMesh(modelGUID, meshIndex, mesh);

		auto vertices = mesh->vertices;
		for (unsigned int i = 0; i < aMesh->mNumVertices; i++) {
			aiVector3D aPosition = aMesh->mVertices[i];
			aiVector3D aNormal = aMesh->mNormals[i];
			Vertex* vertex = new Vertex();
			if (aMesh->mTextureCoords[0] != nullptr) {
				vec2 texCoord;
				texCoord.x = aMesh->mTextureCoords[0][i].x;
				texCoord.y = aMesh->mTextureCoords[0][i].y;
				if (texCoord.x > 1.0f) {
					texCoord.x -= 1.0f;
				}
				if (texCoord.y > 1.0f) {
					texCoord.y -= 1.0f;
				}
				vertex->SetTexCoords(texCoord.x, texCoord.y);
			}
			else {
				vertex->SetTexCoords(0, 0);
			}

			vertex->SetPosition(aPosition.x, aPosition.y, aPosition.z);
			vertex->SetNormal(aNormal.x, aNormal.y, aNormal.z);
			vertices->push_back(vertex);
		}

		vector<unsigned int>* indices = mesh->indices;
		for (unsigned int i = 0; i < aMesh->mNumFaces; i++) {
			aiFace face = aMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices->push_back(face.mIndices[j]);
			}
		}
	}
}

//
//void EditorRendererDomain::DrawModel(const Model* model) {
//	Shader* modelShader = model->material->shader;
//	if (model->isBatched) {
//		IndexBuffer* ib = model->ib_batched;
//		model->va_batched->Bind();
//		ib->Bind();
//		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
//	}
//	else {
//		vector<Mesh*>* allMeshes = model->allMeshes;
//		for (auto mesh : *allMeshes) {
//			Material* meshMaterial = mesh->meshRenderer->material;
//
//			Shader* meshShader = meshMaterial->shader;
//			meshShader = meshShader != nullptr ? meshShader : modelShader;
//			BindShader(model, modelShader);
//
//			Texture* meshDiffuseTexture = meshMaterial->diffuseTexture;
//			Texture* meshSpecularTexture = meshMaterial->specularTexture;
//			if (meshDiffuseTexture != nullptr) meshDiffuseTexture->Bind(TEX_SLOT_DIFFUSE_MAP);
//			if (meshSpecularTexture != nullptr)meshSpecularTexture->Bind(TEX_SLOT_SPECULAR_MAP);
//
//			IndexBuffer* ib = mesh->meshRenderer->ib;
//			mesh->meshRenderer->va->Bind();
//			ib->Bind();
//			GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
//		}
//	}
//}
//
//void EditorRendererDomain::DrawModel(const Model* model, const Material* material) {
//	if (model->isBatched) {
//		IndexBuffer* ib = model->ib_batched;
//		model->va_batched->Bind();
//		ib->Bind();
//		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
//	}
//	else {
//		vector<Mesh*>* allMeshes = model->allMeshes;
//		for (auto mesh : *allMeshes) {
//			BindShader(model, material->shader);
//			if (material->diffuseTexture != nullptr)material->diffuseTexture->Bind(TEX_SLOT_DIFFUSE_MAP);
//			if (material->specularTexture != nullptr)material->specularTexture->Bind(TEX_SLOT_SPECULAR_MAP);
//
//			IndexBuffer* ib = mesh->meshRenderer->ib;
//			mesh->meshRenderer->va->Bind();
//			ib->Bind();
//			GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
//		}
//	}
//}

void EditorRendererDomain::BindShader(const Transform* transform, Shader* shader) {
	if (shader == nullptr) {
		return;
	}
	vec3 modelPos = transform->GetPosition();
	qua modelRot = transform->GetRotation();

	DirectLight* light = editorContext->sceneDirectLight;
	mat4 lightMVPMatrix = light->GetMVPMatrix_Perspective(modelPos);
	vec3 lightPos = light->transform->GetPosition();
	vec3 lightColor = light->color;
	vec3 lightDirection = -light->GetLightDirection();

	Camera* camera = editorContext->sceneViewCamera;
	mat4 cameraMVPMatrix = shader->useLightingMVP ?
		lightMVPMatrix : camera->GetMVPMatrix_Perspective(modelPos);

	// TODO: BIND AND SET BY THE SHADER META DATA.
	shader->Bind();
	shader->SetUniform1i("u_depthMap", TEX_SLOT_DEPTH_MAP);
	shader->SetUniform1i("u_diffuseMap", TEX_SLOT_DIFFUSE_MAP);
	shader->SetUniform1i("u_specularMap", TEX_SLOT_SPECULAR_MAP);
	shader->SetUniformMat4f("u_mvp", cameraMVPMatrix);
	shader->SetUniformMat4f("u_modRotationMatrix", toMat4(modelRot));
	shader->SetUniform3f("u_modPosition", modelPos.x, modelPos.y, modelPos.z);

	shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
	shader->SetUniform3f("u_lightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
	shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);
	shader->SetUniformMat4f("u_lightMVPMatrix", lightMVPMatrix);
	shader->SetUniform1f("u_nearPlane", camera->nearPlane);
	shader->SetUniform1f("u_farPlane", camera->farPlane);
}

bool EditorRendererDomain::TryLoadMaterialByAssetPath(const string& path, Material*& material) {
	string guid;
	if (!EditorDatabase::TryGetGUIDFromAssetPath(path, guid)) {
		return false;
	}

	return TryLoadMaterialByGUID(guid, material);
}

bool EditorRendererDomain::TryLoadMaterialByGUID(const string& guid, Material*& material) {
	string matPath;
	if (!EditorDatabase::TryGetAssetPathFromGUID(guid, matPath)) {
		cout << " **************** Error: EditorRendererDomain::TryLoadMaterialByGUID: GUID no exist!: " << guid << endl;
		return false;
	}

	MaterialRepo* materialRepo = editorContext->GetMaterialRepo();
	if (materialRepo->TryGetMaterialByGUID(guid, material)) {
		return true;
	}

	MaterialMeta materialMeta = MaterialMeta();
	MaterialMeta_DeserializeFrom(&materialMeta, matPath);

	material = new Material();
	string shaderGUID = materialMeta.shaderGUID;

	ShaderRepo* shaderRepo = editorContext->GetShaderRepo();
	if (!shaderRepo->TryGetShaderByGUID(shaderGUID, material->shader)) {
		string shaderPath;
		if (EditorDatabase::TryGetAssetPathFromGUID(shaderGUID, shaderPath)) {
			material->shader = new Shader(shaderPath);
			shaderRepo->TryAddShader(shaderGUID, material->shader);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByGUID: Shader GUID no exist!: " << shaderGUID << endl;
		}
	}

	TextureRepo* textureRepo = editorContext->GetTextureRepo();

	string diffuseTextureGUID = materialMeta.diffuseTextureGUID;
	if (!textureRepo->TryGetTextureByGUID(diffuseTextureGUID, material->diffuseTexture)) {
		string texturePath;
		if (EditorDatabase::TryGetAssetPathFromGUID(diffuseTextureGUID, texturePath)) {
			material->diffuseTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(diffuseTextureGUID, material->diffuseTexture);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByGUID: Texture[Diffuse] GUID no exist!: " << diffuseTextureGUID << endl;
		}
	}

	string specularTextureGUID = materialMeta.specularTextureGUID;
	if (!textureRepo->TryGetTextureByGUID(specularTextureGUID, material->specularTexture)) {
		string texturePath;
		if (EditorDatabase::TryGetAssetPathFromGUID(specularTextureGUID, texturePath)) {
			material->specularTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(specularTextureGUID, material->specularTexture);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByGUID: Texture[Specular] GUID no exist!: " << specularTextureGUID << endl;
		}
	}

	material->diffuseColor = materialMeta.diffuseColor;
	material->specularIntensity = materialMeta.specularIntensity;
	material->shininess = materialMeta.shininess;

	return 	materialRepo->TryAddMaterial(guid, material);
}

SkinMeshRenderer* EditorRendererDomain::LoadSkinMeshRenderer(const aiScene* aScene, PrefabMeta& prefabMeta) {
	SkinMeshRenderer* skinMeshRenderer = new SkinMeshRenderer();
	SkinMeshRendererMeta* skinMeshRendererMeta = prefabMeta.GetComponentMeta<SkinMeshRendererMeta>();
	MeshRepo* meshRepo = editorContext->GetMeshRepo();

	vector<MeshFilter*>* meshFilters = skinMeshRenderer->meshFilters;
	for (size_t i = 0; i < skinMeshRendererMeta->meshFilterMetas.size(); i++) {

		MeshFilterMeta* meshFilterMeta = skinMeshRendererMeta->meshFilterMetas[i];
		Mesh* mesh;
		string modelGUID = meshFilterMeta->modelGUID;
		int meshIndex = meshFilterMeta->meshIndex;

		if (!meshRepo->TryGetMesh(modelGUID, meshIndex, mesh)) {
			mesh = new Mesh();
			aiMesh* amesh = aScene->mMeshes[meshIndex];
			mesh->meshName = amesh->mName.C_Str();
			meshRepo->TryAddMesh(modelGUID, meshIndex, mesh);
		}

		aiMesh* aMesh = aScene->mMeshes[meshIndex];
		auto vertices = mesh->vertices;
		for (unsigned int i = 0; i < aMesh->mNumVertices; i++) {
			aiVector3D aPosition = aMesh->mVertices[i];
			aiVector3D aNormal = aMesh->mNormals[i];
			Vertex* vertex = new Vertex();
			if (aMesh->mTextureCoords[0] != nullptr) {
				vec2 texCoord;
				texCoord.x = aMesh->mTextureCoords[0][i].x;
				texCoord.y = aMesh->mTextureCoords[0][i].y;
				if (texCoord.x > 1.0f) {
					texCoord.x -= 1.0f;
				}
				if (texCoord.y > 1.0f) {
					texCoord.y -= 1.0f;
				}
				vertex->SetTexCoords(texCoord.x, texCoord.y);
			}
			else {
				vertex->SetTexCoords(0, 0);
			}

			vertex->SetPosition(aPosition.x, aPosition.y, aPosition.z);
			vertex->SetNormal(aNormal.x, aNormal.y, aNormal.z);
			vertices->push_back(vertex);
		}

		vector<unsigned int>* indices = mesh->indices;
		for (unsigned int i = 0; i < aMesh->mNumFaces; i++) {
			aiFace face = aMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices->push_back(face.mIndices[j]);
			}
		}

		MeshFilter* meshFilter = new MeshFilter();
		meshFilter->transform = skinMeshRenderer->transform;
		meshFilter->gameObject = skinMeshRenderer->gameObject;
		meshFilter->mesh = mesh;
		meshFilters->push_back(meshFilter);

		// Mesh Renderer
		vector<MeshRenderer*>* meshRenderers = skinMeshRenderer->meshRenderers;
		for (auto meshRendererMeta : skinMeshRendererMeta->meshRendererMetas) {
			MeshRenderer* meshRenderer = new MeshRenderer();
			meshRenderer->transform = skinMeshRenderer->transform;
			meshRenderer->gameObject = skinMeshRenderer->gameObject;
			meshRenderer->materialGUID = meshRendererMeta->materialGUID;
			meshRenderer->GenerateRenderer(meshFilter);
			meshRenderers->push_back(meshRenderer);
		}
	}

	return skinMeshRenderer;
}


void EditorRendererDomain::BatchSkinMeshRenderer(SkinMeshRenderer* skinMeshRenderer) {
	vector<float> vertexData;
	vector<unsigned int> indiceArray;
	unsigned int vertexCount = 0;

	for (auto meshFilter : *skinMeshRenderer->meshFilters) {
		vector<Vertex*>* vertices = meshFilter->mesh->vertices;
		for (auto vertex : *vertices) {
			vec3 position = vertex->position;
			vec2 texCoords = vertex->texCoords;
			vec3 normal = vertex->normal;
			vertexData.push_back(position.x);
			vertexData.push_back(position.y);
			vertexData.push_back(position.z);
			vertexData.push_back(texCoords.x);
			vertexData.push_back(texCoords.y);
		}
		vector<unsigned int>* indices = meshFilter->mesh->indices;
		for (auto indice : *indices) {
			indiceArray.push_back(indice + vertexCount);
		}

		vertexCount += vertices->size();
	}

	VertexArray* va_batched = skinMeshRenderer->va_batched;
	VertexBuffer* vb_batched = skinMeshRenderer->vb_batched;
	IndexBuffer* ib_batched = skinMeshRenderer->ib_batched;
	VertexBufferLayout* vbLayout_batched = skinMeshRenderer->vbLayout_batched;
	va_batched->Bind();
	vb_batched->Ctor(vertexData.data(), vertexData.size());
	va_batched->AddBuffer(vb_batched, vbLayout_batched);
	ib_batched->Ctor(indiceArray.data(), indiceArray.size());

	skinMeshRenderer->isBatched = true;
	cout << "Model BatchSkinMeshRenderer: Vertex float count: " << vertexData.size() << " Indice float count: " << indiceArray.size() << endl;
} 

void EditorRendererDomain::BatchedDrawSkinMeshRenderer(SkinMeshRenderer* skinMeshRenderer) {
	skinMeshRenderer->va_batched->Bind();
	skinMeshRenderer->ib_batched->Bind();
	glDrawElements(GL_TRIANGLES, skinMeshRenderer->ib_batched->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void EditorRendererDomain::LoadScene(const string& path) {
	SceneRepo* sceneRepo = editorContext->GetSceneRepo();
	Scene* scene;
	if (!sceneRepo->TryGetScene(path, scene)) {
		scene = new Scene();
		sceneRepo->TryAddScene(path, scene);
	}

	SceneMeta sceneMeta;
	SceneMeta_DeserializeFrom(&sceneMeta, path);
	sceneMeta.ToScene(*scene);

	for (auto go : scene->gameObjects) {
		vector<SkinMeshRenderer*>& skinMeshRenderers = go->GetAllComponents<SkinMeshRenderer>();
		for (auto skinMeshRenderer : skinMeshRenderers) {
			DrawSkinMeshRenderer(skinMeshRenderer);
		}
	}
}

void EditorRendererDomain::DrawSkinMeshRenderer(const SkinMeshRenderer* skinMeshRenderer) {
	auto meshRenderers = skinMeshRenderer->meshRenderers;
	for (auto meshRenderer : *meshRenderers) {
		DrawMeshRenderer(meshRenderer);
	}
}

void EditorRendererDomain::DrawMeshRenderer(const MeshRenderer* meshRenderer) {
	const Transform* transfrom = meshRenderer->transform;
	Material* material;
	if (TryLoadMaterialByGUID(meshRenderer->materialGUID, material)) {
		BindShader(transfrom, material->shader);
		if (material->diffuseTexture != nullptr)material->diffuseTexture->Bind(TEX_SLOT_DIFFUSE_MAP);
		if (material->specularTexture != nullptr)material->specularTexture->Bind(TEX_SLOT_SPECULAR_MAP);
	}

	IndexBuffer* ib = meshRenderer->ib;
	meshRenderer->va->Bind();
	ib->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}

