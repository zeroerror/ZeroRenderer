#include "EditorRendererDomain.h"
#include "GLDebug.h"
#include "Database.h"
#include "ShaderMeta.h"
#include "FileSuffix.h"
#include "Serialization.h"
#include <assimp/postprocess.h>
#include <src/vendor/glm/gtx/string_cast.hpp>

using namespace Serialization;

void EditorRendererDomain::Inject(EditorContext* ctxt) {
	this->editorContext = ctxt;
}

void EditorRendererDomain::DrawModel(const Model* model) {
	Shader* modelShader = model->material->shader;
	if (model->isBatched) {
		IndexBuffer* ib = model->ib_batched;
		model->va_batched->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	else {
		vector<Mesh*>* allMeshes = model->allMeshes;
		for (auto mesh : *allMeshes) {
			Material* meshMaterial = mesh->meshRenderer->material;

			Shader* meshShader = meshMaterial->shader;
			meshShader = meshShader != nullptr ? meshShader : modelShader;
			BindShader(model, modelShader);

			Texture* meshDiffuseTexture = meshMaterial->diffuseTexture;
			Texture* meshSpecularTexture = meshMaterial->specularTexture;
			if (meshDiffuseTexture != nullptr) meshDiffuseTexture->Bind(TEX_SLOT_DIFFUSE_MAP);
			if (meshSpecularTexture != nullptr)meshSpecularTexture->Bind(TEX_SLOT_SPECULAR_MAP);

			IndexBuffer* ib = mesh->meshRenderer->ib;
			mesh->meshRenderer->va->Bind();
			ib->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
}

void EditorRendererDomain::DrawModel(const Model* model, const Material* material) {
	if (model->isBatched) {
		IndexBuffer* ib = model->ib_batched;
		model->va_batched->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	else {
		vector<Mesh*>* allMeshes = model->allMeshes;
		for (auto mesh : *allMeshes) {
			BindShader(model, material->shader);
			if (material->diffuseTexture != nullptr)material->diffuseTexture->Bind(TEX_SLOT_DIFFUSE_MAP);
			if (material->specularTexture != nullptr)material->specularTexture->Bind(TEX_SLOT_SPECULAR_MAP);

			IndexBuffer* ib = mesh->meshRenderer->ib;
			mesh->meshRenderer->va->Bind();
			ib->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
}

void EditorRendererDomain::BindShader(const Model* model, Shader* shader) {
	if (shader == nullptr) {
		return;
	}
	glm::vec3 modelPos = model->transform->GetPosition();
	glm::qua modelRot = model->transform->GetRotation();

	DirectLight* light = editorContext->sceneDirectLight;
	glm::mat4 lightMVPMatrix = light->GetMVPMatrix_Perspective(modelPos);
	glm::vec3 lightPos = light->transform->GetPosition();
	glm::vec3 lightColor = light->color;
	glm::vec3 lightDirection = -light->GetLightDirection();

	Camera3D* camera = editorContext->sceneViewCamera;
	glm::mat4 cameraMVPMatrix = shader->useLightingMVP ?
		lightMVPMatrix : camera->GetMVPMatrix_Perspective(modelPos);

	// TODO: BIND AND SET BY THE SHADER META DATA.
	shader->Bind();
	shader->SetUniform1i("u_depthMap", TEX_SLOT_DEPTH_MAP);
	shader->SetUniform1i("u_diffuseMap", TEX_SLOT_DIFFUSE_MAP);
	shader->SetUniform1i("u_specularMap", TEX_SLOT_SPECULAR_MAP);
	shader->SetUniformMat4f("u_mvp", cameraMVPMatrix);
	shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(modelRot));
	shader->SetUniform3f("u_modPosition", modelPos.x, modelPos.y, modelPos.z);

	shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
	shader->SetUniform3f("u_lightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
	shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);
	shader->SetUniformMat4f("u_lightMVPMatrix", lightMVPMatrix);
	shader->SetUniform1f("u_nearPlane", camera->nearPlane);
	shader->SetUniform1f("u_farPlane", camera->farPlane);
}

bool EditorRendererDomain::TryLoadMaterialByGUID(const string& guid, Material*& material) {
	string matPath;
	if (!Database::TryGetAssetPathFromGUID(guid, matPath)) {
		cout << " **************** Error: EditorRendererDomain::TryLoadMaterialByGUID: GUID no exist!: " << guid << endl;
		return false;
	}

	MaterialRepo* materialRepo = editorContext->GetMaterialRepo();
	if (materialRepo->TryGetMaterialByGUID(guid, material)) {
		return true;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(matPath);
	material = new Material();
	string shaderGUID = mat.shaderGUID;

	ShaderRepo* shaderRepo = editorContext->GetShaderRepo();
	if (!shaderRepo->TryGetShaderByGUID(shaderGUID, material->shader)) {
		string shaderPath;
		if (Database::TryGetAssetPathFromGUID(shaderGUID, shaderPath)) {
			material->shader = new Shader(shaderPath);
			shaderRepo->TryAddShader(shaderGUID, material->shader);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByGUID: Shader GUID no exist!: " << shaderGUID << endl;
		}
	}

	TextureRepo* textureRepo = editorContext->GetTextureRepo();

	string diffuseTextureGUID = mat.diffuseTextureGUID;
	if (!textureRepo->TryGetTextureByGUID(diffuseTextureGUID, material->diffuseTexture)) {
		string texturePath;
		if (Database::TryGetAssetPathFromGUID(diffuseTextureGUID, texturePath)) {
			material->diffuseTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(diffuseTextureGUID, material->diffuseTexture);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByGUID: Texture[Diffuse] GUID no exist!: " << diffuseTextureGUID << endl;
		}
	}

	string specularTextureGUID = mat.specularTextureGUID;
	if (!textureRepo->TryGetTextureByGUID(specularTextureGUID, material->specularTexture)) {
		string texturePath;
		if (Database::TryGetAssetPathFromGUID(specularTextureGUID, texturePath)) {
			material->specularTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(specularTextureGUID, material->specularTexture);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByGUID: Texture[Specular] GUID no exist!: " << specularTextureGUID << endl;
		}
	}

	material->diffuseColor = mat.diffuseColor;
	material->specularIntensity = mat.specularIntensity;
	material->shininess = mat.shininess;

	return 	materialRepo->TryAddMaterial(guid, material);
}

bool EditorRendererDomain::TryLoadMaterialByAssetPath(const string& path, Material*& material) {
	string guid;
	if (!Database::TryGetGUIDFromAssetPath(path, guid)) {
		cout << " **************** Error: EditorRendererDomain::TryLoadMaterialByAssetPath: Asset path no exist!: " << path << endl;
		return false;
	}

	MaterialRepo* materialRepo = editorContext->GetMaterialRepo();
	if (materialRepo->TryGetMaterialByGUID(guid, material)) {
		return true;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(path);
	material = new Material();
	string shaderGUID = mat.shaderGUID;
	string diffuseTextureGUID = mat.diffuseTextureGUID;

	ShaderRepo* shaderRepo = editorContext->GetShaderRepo();
	if (!shaderRepo->TryGetShaderByGUID(shaderGUID, material->shader)) {
		string shaderPath;
		if (Database::TryGetAssetPathFromGUID(shaderGUID, shaderPath)) {
			Shader* shader = new Shader(shaderPath);
			ShaderMeta shaderMeta = ShaderMeta();
			ShaderMeta_DeserializeFrom(&shaderMeta, shaderPath + FileSuffix::SUFFIX_META);
			shader->useLightingMVP = shaderMeta.useLightingMVP;
			material->shader = shader;
			shaderRepo->TryAddShader(shaderGUID, material->shader);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByAssetPath: Shader GUID no exist!: " << shaderGUID << endl;
		}
	}

	TextureRepo* textureRepo = editorContext->GetTextureRepo();
	if (!textureRepo->TryGetTextureByGUID(diffuseTextureGUID, material->diffuseTexture)) {
		string texturePath;
		if (Database::TryGetAssetPathFromGUID(diffuseTextureGUID, texturePath)) {
			material->diffuseTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(diffuseTextureGUID, material->diffuseTexture);
		}
		else {
			cout << " ################ Warning: EditorRendererDomain::TryLoadMaterialByAssetPath: Texture[Diffuse] GUID no exist!: " << diffuseTextureGUID << endl;
		}
	}

	material->diffuseColor = mat.diffuseColor;
	material->specularIntensity = mat.specularIntensity;
	material->shininess = mat.shininess;

	materialRepo->TryAddMaterial(guid, material);
	return true;
}

void EditorRendererDomain::LoadDefaultScene() {
	unsigned int scrWidth = 1920;
	unsigned int scrHeight = 1080;

	Material* defaultMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/default.mat", defaultMaterial);
	Material* defaultLightMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/defaultLight.mat", defaultLightMaterial);
	Material* lightCubeMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/lightCube.mat", lightCubeMaterial);
	Material* depthMapMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/depthMap.mat", depthMapMaterial);

	// ======================== Scene
	Scene* scene = new Scene();

	GameObject* cameraGO = new GameObject();
	cameraGO->name = "Camera";
	Camera3D* camera = cameraGO->AddComponent<Camera3D>();
	camera->scrWidth = scrWidth;
	camera->scrHeight = scrHeight;
	camera->transform->SetPosition(glm::vec3(0, 10, -10));
	camera->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f))));
	scene->gameObjects.push_back(cameraGO);

	GameObject* directLightGO = new GameObject();
	directLightGO->name = "DirectLight";
	DirectLight* directLight = cameraGO->AddComponent<DirectLight>();
	directLight->scrWidth = scrWidth;
	directLight->scrHeight = scrHeight;
	directLight->shadowType = ShadowType::Hard;
	directLight->transform->SetPosition(glm::vec3(0, 10.0f, 0));
	directLight->transform->SetRotation(glm::quat(glm::vec3(0, 0.5f, 0)));
	directLight->fov = camera->fov;
	directLight->scrWidth = camera->scrWidth;
	directLight->scrHeight = camera->scrHeight;
	directLight->nearPlane = camera->nearPlane;
	directLight->farPlane = camera->farPlane;
	directLight->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	scene->gameObjects.push_back(directLightGO);

	// Create a depth map 2D image
	GameObject* depthMapImageGO = new GameObject();
	depthMapImageGO->name = "DepthMapImage";
	Rectangle* depthMapImage = cameraGO->AddComponent<Rectangle>();
	scene->gameObjects.push_back(depthMapImageGO);
	depthMapImage = Rectangle::CreateRectangle(16.0f, 9.0f);
	depthMapImage->transform->SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	depthMapImage->transform->SetRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	depthMapImage->material = depthMapMaterial;

	// Create a central light source cube
	GameObject* lightCubeGO = new GameObject();
	lightCubeGO->name = "LightCube";
	Cube* lightCube = lightCubeGO->AddComponent<Cube>();
	lightCube->Ctor(0.2f, 0.2f, 1.0f);
	lightCube->material = lightCubeMaterial;

	// Create the ground
	GameObject* groundCubeGO = new GameObject();
	groundCubeGO->name = "GroundCube";
	Cube* groundCube = lightCubeGO->AddComponent<Cube>();
	groundCube->Ctor(20.0f, 0.1f, 30.0f);
	groundCube->transform->SetPosition(glm::vec3(0.0f, -0.05f, 0.0f));
	groundCube->material = defaultLightMaterial;
	scene->gameObjects.push_back(groundCubeGO);

	// Create walls
	GameObject* wall1GO = new GameObject();
	wall1GO->name = "Wall1";
	Cube* wall1 = lightCubeGO->AddComponent<Cube>();
	wall1->Ctor(1.0f, 5.0f, 10.0f);
	wall1->transform->SetPosition(glm::vec3(-8.0f, 2.5f, 0.0f));
	wall1->material = defaultLightMaterial;
	scene->gameObjects.push_back(wall1GO);

	GameObject* wall2GO = new GameObject();
	wall2GO->name = "wall2";
	Cube* wall2 = lightCubeGO->AddComponent<Cube>();
	wall2->Ctor(10.0f, 5.0f, 1.0f);
	wall2->transform->SetPosition(glm::vec3(0.0f, 2.5f, -8.0f));
	wall2->material = defaultLightMaterial;
	scene->gameObjects.push_back(wall2GO);

	// Create obstacles
	GameObject* obstacle1GO = new GameObject();
	obstacle1GO->name = "obstacle1";
	Cube* obstacle1 = lightCubeGO->AddComponent<Cube>();
	obstacle1->Ctor(2.0f, 2.0f, 2.0f);
	obstacle1->transform->SetPosition(glm::vec3(-4.0f, 1.0f, 4.0f));
	obstacle1->material = defaultLightMaterial;
	scene->gameObjects.push_back(obstacle1GO);

	GameObject* obstacle2GO = new GameObject();
	obstacle2GO->name = "obstacle2";
	Cube* obstacle2 = lightCubeGO->AddComponent<Cube>();
	obstacle2->Ctor(2.0f, 2.0f, 2.0f);
	obstacle2->transform->SetPosition(glm::vec3(4.0f, 1.0f, -4.0f));
	obstacle2->material = defaultLightMaterial;
	scene->gameObjects.push_back(obstacle2GO);

	GameObject* obstacle3GO = new GameObject();
	obstacle3GO->name = "obstacle3";
	Cube* obstacle3 = lightCubeGO->AddComponent<Cube>();
	obstacle3->Ctor(3.0f, 1.0f, 2.0f);
	obstacle3->transform->SetPosition(glm::vec3(6.0f, 0.5f, 6.0f));
	obstacle3->material = defaultLightMaterial;
	scene->gameObjects.push_back(obstacle3GO);

	// ========================== Load Model
	Model* model;
	if (TryLoadModel("asset/model/nanosuit/nanosuit.obj", model)) {
		model->transform->SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
		model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
		model->material = defaultLightMaterial;

		GameObject* modelPrefab = new GameObject();
		// Todo: Model to prefab
		scene->gameObjects.push_back(modelPrefab);
	}
	if (TryLoadModel("asset/model/nanosuit/nanosuit.obj", model)) {
		model->transform->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
		model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
		model->material = defaultLightMaterial;

		GameObject* modelPrefab = new GameObject();
		// Todo: Model to prefab
		scene->gameObjects.push_back(modelPrefab);
	}

	editorContext->currentScene = scene;
}


bool EditorRendererDomain::TryLoadModel(const string& path, Model*& model) {
	model = nullptr;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "  ################################# ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return false;
	}

	string modelMetaPath = path + FileSuffix::SUFFIX_META;
	ObjMeta objMeta = ObjMeta();
	ObjMeta_DeserializeFrom(&objMeta, modelMetaPath);
	size_t materialIndex = 0;
	model = new Model();
	ProcessNode(scene->mRootNode, scene, objMeta, model->allMeshes, materialIndex);
	return true;
}

void EditorRendererDomain::ProcessNode(aiNode* aNode, const aiScene* aScene, const ObjMeta& objMeta, vector<Mesh*>* allMeshes, size_t& materialIndex) {
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		allMeshes->push_back(ProcessMesh(mesh, aScene, objMeta, materialIndex));
	}
	for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
		aiNode* childNode = aNode->mChildren[i];
		ProcessNode(childNode, aScene, objMeta, allMeshes, materialIndex);
	}
}

Mesh* EditorRendererDomain::ProcessMesh(aiMesh* aiMesh, const aiScene* aScene, const ObjMeta& objMeta, size_t& materialIndex) {
	Mesh* mesh = new Mesh();

	string materialGUID = objMeta.materialGUIDs[materialIndex++];
	mesh->meshRenderer->materialGUID = materialGUID;
	TryLoadMaterialByGUID(materialGUID, mesh->meshRenderer->material);

	vector<Vertex*>* vertices = mesh->meshFilter->vertices;

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
		aiVector3D aPosition = aiMesh->mVertices[i];
		aiVector3D aNormal = aiMesh->mNormals[i];
		Vertex* vertex = new Vertex();
		if (aiMesh->mTextureCoords[0] != nullptr) {
			glm::vec2 texCoord;
			texCoord.x = aiMesh->mTextureCoords[0][i].x;
			texCoord.y = aiMesh->mTextureCoords[0][i].y;
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

	vector<unsigned int>* indices = mesh->meshFilter->indices;
	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
		aiFace face = aiMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices->push_back(face.mIndices[j]);
		}
	}

	mesh->GenerateRenderer();

	return mesh;
}

