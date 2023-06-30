#include "EditorRendererDomain.h"
#include "GLDebug.h"
#include "Database.h"
#include "ShaderMetadata.h"
#include "FileSuffix.h"

void EditorRendererDomain::Inject(EditorContext* ctxt) {
	this->editorContext = ctxt;
}

void EditorRendererDomain::DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light) {
	BindMaterial(model, model->material);
	_DrawModel(model, camera, light);
}

void EditorRendererDomain::DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light, const Material* material) {
	BindMaterial(model, material);
	_DrawModel(model, camera, light);
}

void EditorRendererDomain::_DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light) {
	if (model->isBatched) {
		IndexBuffer* ib = model->ib_batched;
		model->va_batched->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	else {
		MaterialRepo* materialRepo = editorContext->GetMaterialRepo();
		vector<Mesh*>* allMeshes = model->allMeshes;
		for (auto mesh : *allMeshes) {
			IndexBuffer* ib = mesh->ib;
			mesh->va->Bind();
			ib->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
}

void EditorRendererDomain::BindMaterial(const Model* model, const Material* material) {
	Shader* shader = material->shader;
	if (shader != nullptr) {
		shader->Bind();

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
		shader->SetUniform1i("u_texture", 1);
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

	Texture* texture = material->diffuseTexture;
	if (texture != nullptr)	texture->Bind(1);
}

bool EditorRendererDomain::TryLoadMaterialByGUID(const string& guid, Material*& material) {
	if (!Database::GUIDExist(guid)) {
		return false;
	}

	MaterialRepo* materialRepo = editorContext->GetMaterialRepo();
	if (materialRepo->TryGetMaterialByGUID(guid, material)) {
		return true;
	}

	string matPath;
	if (!Database::TryGetAssetPathFromGUID(guid, matPath)) {
		return false;
	}

	Mat mat = Mat();
	mat.DeserializeFrom(matPath);
	material = new Material();
	string shaderGUID = mat.shaderGUID;
	string diffuseTextureGUID = mat.diffuseTextureGUID;

	ShaderRepo* shaderRepo = editorContext->GetShaderRepo();
	if (!shaderRepo->TryGetShaderByGUID(shaderGUID, material->shader)) {
		string shaderPath;
		if (Database::TryGetAssetPathFromGUID(shaderGUID, shaderPath)) {
			material->shader = new Shader(shaderPath);
			shaderRepo->TryAddShader(shaderGUID, material->shader);
		}
	}

	TextureRepo* textureRepo = editorContext->GetTextureRepo();
	if (!textureRepo->TryGetTextureByGUID(diffuseTextureGUID, material->diffuseTexture)) {
		string texturePath;
		if (Database::TryGetAssetPathFromGUID(diffuseTextureGUID, texturePath)) {
			material->diffuseTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(diffuseTextureGUID, material->diffuseTexture);
		}
	}

	material->diffuseColor = mat.diffuseColor;
	material->specularIntensity = mat.specularIntensity;
	material->shininess = mat.shininess;

	return 	materialRepo->TryAddMaterial(guid, material);
}

bool EditorRendererDomain::TryLoadMaterialByAssetPath(const string& path, Material*& material) {
	if (!Database::AssetPathExist(path)) {
		return false;
	}

	string guid;
	Database::TryGetGUIDFromAssetPath(path, guid);

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
			ShaderMetadata shaderMetadata = ShaderMetadata();
			shaderMetadata.DeserializeFrom(shaderPath + FileSuffix::SUFFIX_META);
			shader->useLightingMVP = shaderMetadata.useLightingMVP;
			material->shader = shader;
			shaderRepo->TryAddShader(shaderGUID, material->shader);
		}
	}

	TextureRepo* textureRepo = editorContext->GetTextureRepo();
	if (!textureRepo->TryGetTextureByGUID(diffuseTextureGUID, material->diffuseTexture)) {
		string texturePath;
		if (Database::TryGetAssetPathFromGUID(diffuseTextureGUID, texturePath)) {
			material->diffuseTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(diffuseTextureGUID, material->diffuseTexture);
		}
	}

	material->diffuseColor = mat.diffuseColor;
	material->specularIntensity = mat.specularIntensity;
	material->shininess = mat.shininess;
	material->assetPath = path;

	materialRepo->TryAddMaterial(guid, material);
	return true;
}

void EditorRendererDomain::LoadDefaultScene() {
	unsigned int scrWidth = 1920;
	unsigned int scrHeight = 1080;

	// ======================== Scene
	Scene* scene = new Scene();

	Camera3D* camera = scene->camera;
	camera->scrWidth = scrWidth;
	camera->scrHeight = scrHeight;
	camera->transform->SetPosition(glm::vec3(0, 10, -10));
	camera->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f))));

	// Light 
	DirectLight* directLight = new DirectLight();
	directLight->scrWidth = scrWidth;
	directLight->scrHeight = scrHeight;
	directLight->shadowType = ShadowType::Hard;
	directLight->transform->SetPosition(camera->transform->GetPosition());
	directLight->transform->SetRotation(camera->transform->GetRotation());
	directLight->fov = camera->fov;
	directLight->scrWidth = camera->scrWidth;
	directLight->scrHeight = camera->scrHeight;
	directLight->nearPlane = camera->nearPlane;
	directLight->farPlane = camera->farPlane;
	directLight->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	scene->directLight = directLight;

	Material* defaultMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/default.mat", defaultMaterial);
	Material* defaultLightMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/defaultLight.mat", defaultLightMaterial);
	Material* lightCubeMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/lightCube.mat", lightCubeMaterial);
	Material* depthMapMaterial = new Material();
	TryLoadMaterialByAssetPath("asset/material/depthMap.mat", depthMapMaterial);

	// Create a central light source cube
	scene->lightCube = Cube::CreateCube(0.2f, 0.2f, 1.0f);
	scene->lightCube->material = lightCubeMaterial;

	// Create a depth map 2D image
	scene->depthMapImage = Rectangle::CreateRectangle(16.0f, 9.0f);
	scene->depthMapImage->transform->SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));
	scene->depthMapImage->transform->SetRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	scene->depthMapImage->material = depthMapMaterial;

	// Create the ground
	Cube* groundCube = Cube::CreateCube(20.0f, 0.1f, 30.0f);
	groundCube->transform->SetPosition(glm::vec3(0.0f, -0.05f, 0.0f));
	groundCube->material = defaultLightMaterial;
	scene->cubes->push_back(groundCube);

	// Create walls
	Cube* wall1 = Cube::CreateCube(1.0f, 5.0f, 10.0f);
	wall1->transform->SetPosition(glm::vec3(-8.0f, 2.5f, 0.0f));
	wall1->material = defaultLightMaterial;
	scene->cubes->push_back(wall1);

	Cube* wall2 = Cube::CreateCube(10.0f, 5.0f, 1.0f);
	wall2->transform->SetPosition(glm::vec3(0.0f, 2.5f, -8.0f));
	wall2->material = defaultLightMaterial;
	scene->cubes->push_back(wall2);

	// Create obstacles
	Cube* obstacle1 = Cube::CreateCube(2.0f, 2.0f, 2.0f);
	obstacle1->transform->SetPosition(glm::vec3(-4.0f, 1.0f, 4.0f));
	obstacle1->material = defaultLightMaterial;
	scene->cubes->push_back(obstacle1);

	Cube* obstacle2 = Cube::CreateCube(2.0f, 2.0f, 2.0f);
	obstacle2->transform->SetPosition(glm::vec3(4.0f, 1.0f, -4.0f));
	obstacle2->material = defaultLightMaterial;
	scene->cubes->push_back(obstacle2);

	Cube* obstacle3 = Cube::CreateCube(3.0f, 1.0f, 2.0f);
	obstacle3->transform->SetPosition(glm::vec3(6.0f, 0.5f, 6.0f));
	obstacle3->material = defaultLightMaterial;
	scene->cubes->push_back(obstacle3);

	// ========================== Load Model
	Model* model = new Model();
	model->LoadModel("asset/model/nanosuit/nanosuit.obj");
	model->transform->SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));
	model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
	model->material = defaultLightMaterial;
	scene->models->push_back(model);

	editorContext->currentScene = scene;
}
