#include "RuntimeDomain.h"
#include "GLDebug.h"
#include "EditorDatabase.h"
#include "ShaderMeta.h"
#include "FileSuffix.h"
#include "Scene.h"
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

void RuntimeDomain::Inject(RuntimeContext *ctxt)
{
	this->_runtimeContext = ctxt;
}

void RuntimeDomain::PreprocessModelMeshes()
{
	// Load all models'meshes to mesh repo.
	vector<string> suffixes = vector<string>();
	suffixes.push_back(FileSuffix::SUFFIX_OBJ);
	unsigned int suffixFlag = FileSuffix::ToFileSuffixFlag(suffixes);

	vector<string> filePaths;
	FileHelper::GetFilePaths("asset", suffixFlag, filePaths);
	for (auto path : filePaths)
	{
		PreprocessModelMeshFromPath(path);
	}
}

void RuntimeDomain::PreprocessModelMeshFromPath(const string &path)
{
	const aiScene *aScene = nullptr;
	if (!EditorModelManager::TryLoadModel(path, aScene))
	{
		return;
	}

	PrefabMeta prefabMeta = PrefabMeta();
	string prefabPath = path.substr(0, path.find_last_of(".")) + FileSuffix::SUFFIX_PREFAB;
	PrefabMeta_DeserializeFrom(prefabMeta, prefabPath);

	SkinMeshRendererMeta *skinMeshRendererMeta = prefabMeta.GetComponentMeta<SkinMeshRendererMeta>();
	PreprocessSMRMetaToMesh(aScene, *skinMeshRendererMeta);
}

void RuntimeDomain::PreprocessSMRMetaToMesh(const aiScene *aScene, const SkinMeshRendererMeta &skinMeshRendererMeta)
{
	MeshRepo *meshRepo = _runtimeContext->GetMeshRepo();

	for (size_t i = 0; i < skinMeshRendererMeta.meshFilterMetas.size(); i++)
	{
		Mesh *mesh = new Mesh();
		auto meshFilterMeta = skinMeshRendererMeta.meshFilterMetas[i];
		ProcessModelToMesh(aScene, meshFilterMeta->meshIndex, mesh);

		string modelGUID = meshFilterMeta->modelGUID;
		int meshIndex = meshFilterMeta->meshIndex;
		meshRepo->TryAddMesh(modelGUID, meshIndex, mesh);
	}
}

void RuntimeDomain::ProcessModelToMesh(const aiScene *aScene, const int &meshIndex, Mesh *mesh)
{
	aiMesh *aMesh = aScene->mMeshes[meshIndex];
	mesh->meshName = aMesh->mName.C_Str();
	auto vertices = mesh->vertices;
	for (unsigned int i = 0; i < aMesh->mNumVertices; i++)
	{
		aiVector3D aPosition = aMesh->mVertices[i];
		aiVector3D aNormal = aMesh->mNormals[i];
		Vertex *vertex = new Vertex();
		if (aMesh->mTextureCoords[0] != nullptr)
		{
			vec2 texCoord;
			texCoord.x = aMesh->mTextureCoords[0][i].x;
			texCoord.y = aMesh->mTextureCoords[0][i].y;
			if (texCoord.x > 1.0f)
			{
				texCoord.x -= 1.0f;
			}
			if (texCoord.y > 1.0f)
			{
				texCoord.y -= 1.0f;
			}
			vertex->SetTexCoords(texCoord.x, texCoord.y);
		}
		else
		{
			vertex->SetTexCoords(0, 0);
		}

		vertex->SetPosition(aPosition.x, aPosition.y, aPosition.z);
		vertex->SetNormal(aNormal.x, aNormal.y, aNormal.z);
		vertices->push_back(vertex);
	}

	vector<unsigned int> *indices = mesh->indices;
	for (unsigned int i = 0; i < aMesh->mNumFaces; i++)
	{
		aiFace face = aMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices->push_back(face.mIndices[j]);
		}
	}
}

void RuntimeDomain::BindShader(const Transform *transform, Shader *shader, const Camera &camera)
{
	if (shader == nullptr)
	{
		return;
	}

	// 模型参数
	vec3 modelPos = transform->GetPosition();
	qua modelRot = transform->GetRotation();
	vec3 modelScale = transform->GetScale();
	mat4 modelRotMatrix = toMat4(modelRot);
	// 摄像机参数
	mat4 camMVP;
	if (camera.cameraType == CameraType::Perspective)
	{
		camMVP = camera.GetMVPMatrix_Perspective(modelPos);
	}
	else if (camera.cameraType == CameraType::Ortho)
	{
		camMVP = camera.GetMVPMatrix_Ortho(modelPos);
	}
	else
	{
		// 深度贴图的绘制 使用正交视角 防止透视投影的透视失真
		camMVP = camera.GetMVPMatrix_Ortho(modelPos);
	}
	// 所有光源参数
	DirectLight *dl = _runtimeContext->sceneDirectLight;
	vec3 dlPos = dl->transform->GetPosition();
	vec3 dlColor = dl->color;
	vec3 dlDir = dl->transform->GetForward();
	mat4 dlMVP = Camera::GetMVPMatrix_Ortho(modelPos, *dl->transform, dl->orthoSize, dl->nearPlane, dl->farPlane);

	shader->Bind();
	// TODO: 从 ShaderMeta 中读取 Uniform
	// ShaderMeta shaderMeta = ShaderMeta();
	// ShaderMeta_DeserializeFrom(&shaderMeta, shader->GetPath());
	// for (int i = 0; i < shaderMeta.uniforms.size(); i++) {
	// 	ShaderUniform uniform = shaderMeta.uniforms[i];
	// 	string uniformName = uniform.name;
	// 	ShaderUniformType_ uniformType = uniform.type;
	// 	std::any uniformValue = uniform.value;
	// 	if (uniformType == ShaderUniformType_Int) {
	// 		shader->SetUniform1i(uniformName, any_cast<int>(uniformValue));
	// 	}
	// 	else if (uniformType == ShaderUniformType_Float) {
	// 		float v = any_cast<float>(uniformValue);
	// 		shader->SetUniform1f(uniformName, v);
	// 	}
	// 	else if (uniformType == ShaderUniformType_Float3) {
	// 		vec3 v = any_cast<vec3>(uniformValue);
	// 		shader->SetUniform3f(uniformName, v.x, v.y, v.z);
	// 	}
	// 	else if (uniformType == ShaderUniformType_Float4) {
	// 		vec4 v = any_cast<vec4>(uniformValue);
	// 		shader->SetUniform4f(uniformName, v.x, v.y, v.z, v.w);
	// 	}
	// 	else if (uniformType == ShaderUniformType_Matrix44) {
	// 		mat4 mat = any_cast<mat4>(uniformValue);
	// 		shader->SetUniformMat4f(uniformName, mat);
	// 	}
	// }

	shader->SetUniform1i("u_depthMap", TEX_SLOT_DEPTH_MAP);
	shader->SetUniform1i("u_diffuseMap", TEX_SLOT_DIFFUSE_MAP);
	shader->SetUniform1i("u_specularMap", TEX_SLOT_SPECULAR_MAP);

	shader->SetUniformMat4f("u_mvp", camMVP);
	shader->SetUniform3f("u_modPosition", modelPos.x, modelPos.y, modelPos.z);
	shader->SetUniformMat4f("u_modRotationMatrix", modelRotMatrix);
	shader->SetUniform3f("u_modScale", modelScale.x, modelScale.y, modelScale.z);

	shader->SetUniform3f("u_lightPosition", dlPos.x, dlPos.y, dlPos.z);
	shader->SetUniform3f("u_lightDirection", dlDir.x, dlDir.y, dlDir.z);
	shader->SetUniform3f("u_lightColor", dlColor.x, dlColor.y, dlColor.z);
	shader->SetUniformMat4f("u_lightMVPMatrix", dlMVP);
	shader->SetUniform1f("u_nearPlane", camera.nearPlane);
	shader->SetUniform1f("u_farPlane", camera.farPlane);
}

bool RuntimeDomain::TryLoadMaterialByAssetPath(const string &path, Material *&material)
{
	string guid;
	if (!EditorDatabase::TryGetGUIDFromAssetPath(path, guid))
	{
		return false;
	}

	return TryLoadMaterialByGUID(guid, material);
}

bool RuntimeDomain::TryLoadMaterialByGUID(const string &guid, Material *&material)
{
	string matPath;
	if (!EditorDatabase::TryGetAssetPathFromGUID(guid, matPath))
	{
		return false;
	}

	MaterialRepo *materialRepo = _runtimeContext->GetMaterialRepo();
	if (materialRepo->TryGetMaterialByGUID(guid, material))
	{
		return true;
	}

	MaterialMeta materialMeta = MaterialMeta();
	MaterialMeta_DeserializeFrom(&materialMeta, matPath);

	material = new Material();
	string shaderGUID = materialMeta.shaderGUID;

	ShaderRepo *shaderRepo = _runtimeContext->GetShaderRepo();
	if (!shaderRepo->TryGetShaderByGUID(shaderGUID, material->shader))
	{
		string shaderPath;
		if (EditorDatabase::TryGetAssetPathFromGUID(shaderGUID, shaderPath))
		{
			material->shader = new Shader(shaderPath);
			shaderRepo->TryAddShader(shaderGUID, material->shader);
		}
	}

	TextureRepo *textureRepo = _runtimeContext->GetTextureRepo();

	string diffuseTextureGUID = materialMeta.diffuseTextureGUID;
	if (!textureRepo->TryGetTextureByGUID(diffuseTextureGUID, material->diffuseTexture))
	{
		string texturePath;
		if (EditorDatabase::TryGetAssetPathFromGUID(diffuseTextureGUID, texturePath))
		{
			material->diffuseTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(diffuseTextureGUID, material->diffuseTexture);
		}
	}

	string specularTextureGUID = materialMeta.specularTextureGUID;
	if (!textureRepo->TryGetTextureByGUID(specularTextureGUID, material->specularTexture))
	{
		string texturePath;
		if (EditorDatabase::TryGetAssetPathFromGUID(specularTextureGUID, texturePath))
		{
			material->specularTexture = new Texture(texturePath);
			textureRepo->TryAddTexture(specularTextureGUID, material->specularTexture);
		}
	}

	material->diffuseColor = materialMeta.diffuseColor;
	material->specularIntensity = materialMeta.specularIntensity;
	material->shininess = materialMeta.shininess;

	return materialRepo->TryAddMaterial(guid, material);
}

void RuntimeDomain::BatchSkinMeshRenderer(SkinMeshRenderer *skinMeshRenderer)
{
	vector<float> vertexData;
	vector<unsigned int> indiceArray;
	unsigned int vertexCount = 0;

	for (auto meshFilter : *skinMeshRenderer->meshFilters)
	{
		vector<Vertex *> *vertices = meshFilter->mesh->vertices;
		for (auto vertex : *vertices)
		{
			vec3 position = vertex->position;
			vec2 texCoords = vertex->texCoords;
			vec3 normal = vertex->normal;
			vertexData.push_back(position.x);
			vertexData.push_back(position.y);
			vertexData.push_back(position.z);
			vertexData.push_back(texCoords.x);
			vertexData.push_back(texCoords.y);
		}
		vector<unsigned int> *indices = meshFilter->mesh->indices;
		for (auto indice : *indices)
		{
			indiceArray.push_back(indice + vertexCount);
		}

		vertexCount += vertices->size();
	}

	VertexArray *va_batched = skinMeshRenderer->va_batched;
	VertexBuffer *vb_batched = skinMeshRenderer->vb_batched;
	IndexBuffer *ib_batched = skinMeshRenderer->ib_batched;
	VertexBufferLayout *vbLayout_batched = skinMeshRenderer->vbLayout_batched;
	va_batched->Bind();
	vb_batched->Ctor(vertexData.data(), vertexData.size());
	va_batched->AddBuffer(vb_batched, vbLayout_batched);
	ib_batched->Ctor(indiceArray.data(), indiceArray.size());

	skinMeshRenderer->isBatched = true;
	cout << "Model BatchSkinMeshRenderer: Vertex float count: " << vertexData.size() << " Indice float count: " << indiceArray.size() << endl;
}

void RuntimeDomain::BatchedDrawSkinMeshRenderer(SkinMeshRenderer *skinMeshRenderer)
{
	skinMeshRenderer->va_batched->Bind();
	skinMeshRenderer->ib_batched->Bind();
	glDrawElements(GL_TRIANGLES, skinMeshRenderer->ib_batched->GetCount(), GL_UNSIGNED_INT, nullptr);
}

Scene *RuntimeDomain::OpenScene(const string &path, SceneMeta &resSceneMeta)
{
	string sceneGUID;
	if (!EditorDatabase::TryGetGUIDFromAssetPath(path, sceneGUID))
	{
		return nullptr;
	}

	SceneRepo *sceneRepo = _runtimeContext->GetSceneRepo();
	Scene *scene;
	if (!sceneRepo->TryGetScene(sceneGUID, scene))
	{
		scene = new Scene();
		_runtimeContext->currentScene = scene;

		sceneRepo->TryAddScene(sceneGUID, scene);
		SceneMeta_DeserializeFrom(&resSceneMeta, path);
		MetaToScene(resSceneMeta, *scene);
	}
	else
	{
		_runtimeContext->currentScene = scene;
	}

	auto mainCam = scene->FindByPath("Root/Camera")->GetComponent<Camera>();
	auto directLight = scene->FindByPath("Root/DirectLight")->GetComponent<DirectLight>();
	_runtimeContext->mainCamera = mainCam;
	_runtimeContext->sceneDirectLight = directLight;

	// Scene Shadow FBO and Texture
	glGenFramebuffers(1, &_runtimeContext->currentSceneShadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _runtimeContext->currentSceneShadowMapFBO);
	glGenTextures(1, &_runtimeContext->currentSceneShadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, _runtimeContext->currentSceneShadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mainCam->scrWidth, mainCam->scrHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _runtimeContext->currentSceneShadowMapTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return scene;
}

inline void _renderGO(RuntimeDomain *domain, GameObject *go, const Camera &camera)
{
	vector<SkinMeshRenderer *> skinMeshRenderers = vector<SkinMeshRenderer *>();
	go->GetAllComponents<SkinMeshRenderer>(skinMeshRenderers);
	if (skinMeshRenderers.size() > 0)
	{
		for (auto skinMeshRenderer : skinMeshRenderers)
		{
			domain->DrawSkinMeshRenderer(skinMeshRenderer, camera);
		}
		return;
	}

	vector<MeshRenderer *> meshRenderers = vector<MeshRenderer *>();
	go->GetAllComponents<MeshRenderer>(meshRenderers);
	for (auto meshRenderer : meshRenderers)
	{
		domain->DrawMeshRenderer(meshRenderer, camera);
	}
}

void RuntimeDomain::RenderScene(const Scene &scene, const Camera &camera)
{
	for (GameObject *go : *scene.rootGameObjects)
	{
		_renderGO(this, go, camera);
		go->dfsChildren([this, camera](GameObject *childGO)
						{ _renderGO(this, childGO, camera); });
	}
}

void RuntimeDomain::RendererSceneShadowMap(const Scene &scene, const Camera &camera)
{
	DirectLight *dl = _runtimeContext->sceneDirectLight;
	if (dl == nullptr)
		return;
	glBindFramebuffer(GL_FRAMEBUFFER, _runtimeContext->currentSceneShadowMapFBO);
	glBindTexture(GL_TEXTURE_2D, _runtimeContext->currentSceneShadowMapTexture);
	glClear(GL_DEPTH_BUFFER_BIT);
	Camera dlCam = Camera();
	dlCam.CopyFrom(camera);
	dlCam.transform = new Transform();
	dlCam.transform->SetPosition(dl->transform->GetPosition());
	dlCam.transform->SetRotation(dl->transform->GetRotation());
	dlCam.cameraType = CameraType::None;
	dlCam.orthoSize = dl->orthoSize;
	RenderScene(scene, dlCam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RuntimeDomain::DrawSkinMeshRenderer(const SkinMeshRenderer *skinMeshRenderer, const Camera &camera)
{
	auto meshRenderers = skinMeshRenderer->meshRenderers;
	for (auto meshRenderer : *meshRenderers)
	{
		DrawMeshRenderer(meshRenderer, camera);
	}
}

void RuntimeDomain::DrawMeshRenderer(const MeshRenderer *meshRenderer, const Camera &camera)
{
	const Transform *transfrom = meshRenderer->transform;
	Material *material;
	// 判定是否为主摄像机
	const Camera *mainCam = _runtimeContext->mainCamera;
	const bool isMainCam = camera.gameObject && mainCam->gameObject->GetGID() == camera.gameObject->GetGID();
	if (!isMainCam)
	{
		material = _runtimeContext->GetMaterialRepo()->errorMat;
	}
	else if (!TryLoadMaterialByGUID(meshRenderer->materialGUID, material))
	{
		material = _runtimeContext->GetMaterialRepo()->defaultMaterial;
	}

	BindShader(transfrom, material->shader, camera);

	// Bind Texture
	// 漫反射贴图
	if (material->diffuseTexture != nullptr)
	{
		material->diffuseTexture->Bind(TEX_SLOT_DIFFUSE_MAP);
	}
	else
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + TEX_SLOT_DIFFUSE_MAP));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	// 高光贴图
	if (material->specularTexture != nullptr)
	{
		material->specularTexture->Bind(TEX_SLOT_SPECULAR_MAP);
	}
	else
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + TEX_SLOT_SPECULAR_MAP));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	// 深度贴图
	GLCall(glActiveTexture(GL_TEXTURE0 + TEX_SLOT_DEPTH_MAP));
	GLCall(glBindTexture(GL_TEXTURE_2D, _runtimeContext->currentSceneShadowMapTexture));

	IndexBuffer *ib = meshRenderer->ib;
	meshRenderer->va->Bind();
	ib->Bind();
	glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void RuntimeDomain::MetaToCamera(const CameraMeta &cameraMeta, Camera &camera)
{
	camera.cameraType = cameraMeta.cameraType;
	camera.nearPlane = cameraMeta.nearPlane;
	camera.farPlane = cameraMeta.farPlane;
	camera.fov = cameraMeta.fov;
	camera.orthoSize = cameraMeta.orthoSize;
	camera.scrWidth = cameraMeta.scrWidth;
	camera.scrHeight = cameraMeta.scrHeight;
}

void RuntimeDomain::MetaToDirectLight(const DirectLightMeta &directLightMeta, DirectLight &directLight)
{
	directLight.color = directLightMeta.color;
	directLight.shadowType = directLightMeta.shadowType;
}

void RuntimeDomain::MetaToMeshFilter(const MeshFilterMeta &meshFilterMeta, MeshFilter &meshFilter)
{
	Mesh *mesh = new Mesh();
	_runtimeContext->GetMeshRepo()->TryGetMesh(meshFilterMeta.modelGUID, meshFilterMeta.meshIndex, mesh);
	meshFilter.mesh = mesh;
	meshFilter.meshIndex_editorCache = meshFilterMeta.meshIndex;
	meshFilter.modelGUID_editorCache = meshFilterMeta.modelGUID;
}

void RuntimeDomain::MetaToMeshRenderer(const MeshRendererMeta &meshRendererMeta, MeshRenderer &meshRenderer)
{
	meshRenderer.materialGUID = meshRenderer.materialGUID;
}

void RuntimeDomain::MetaToSkinMeshRenderer(const SkinMeshRendererMeta &skinMeshRendererMeta, SkinMeshRenderer &skinMeshRenderer)
{
	vector<MeshFilter *> *meshFilters = new vector<MeshFilter *>();
	vector<MeshRenderer *> *meshRenderers = new vector<MeshRenderer *>();

	for (int i = 0; i < skinMeshRendererMeta.meshFilterMetas.size(); i++)
	{
		MeshFilterMeta *mfMeta = skinMeshRendererMeta.meshFilterMetas[i];
		MeshFilter *meshFilter = new MeshFilter();
		meshFilter->gameObject = skinMeshRenderer.gameObject;
		meshFilter->transform = skinMeshRenderer.transform;
		_runtimeContext->GetMeshRepo()->TryGetMesh(mfMeta->modelGUID, mfMeta->meshIndex, meshFilter->mesh);
		meshFilters->push_back(meshFilter);

		MeshRendererMeta *mrMeta = skinMeshRendererMeta.meshRendererMetas[i];
		MeshRenderer *meshRenderer = new MeshRenderer();
		meshRenderer->gameObject = skinMeshRenderer.gameObject;
		meshRenderer->transform = skinMeshRenderer.transform;
		meshRenderer->materialGUID = mrMeta->materialGUID;
		meshRenderer->GenerateRenderer(meshFilter);
		meshRenderers->push_back(meshRenderer);
	}

	skinMeshRenderer.meshFilters = meshFilters;
	skinMeshRenderer.meshRenderers = meshRenderers;
}

void RuntimeDomain::MetaToTransform(const TransformMeta &transformMeta, Transform &transform)
{
	transform.SetPosition(transformMeta.position);
	transform.SetRotation(transformMeta.rotation);
	transform.SetScale(transformMeta.scale);
	transform.fatherGID_forSerialize = transformMeta.fatherGID;
	transform.childrenGIDs_forSerialize = *transformMeta.childrenGIDs;
}

void RuntimeDomain::GUIDToPrefabMeta(const string &guid, PrefabMeta &prefabMeta)
{
	string prefabPath;
	if (!EditorDatabase::TryGetAssetPathFromGUID(guid, prefabPath))
	{
		cout << "RuntimeDomain::GUIDToPrefabMeta: Can't find prefab path by guid: " << guid << endl;
		return;
	}

	PrefabMeta_DeserializeFrom(prefabMeta, prefabPath);
}

inline void RuntimeDomain::_MetaToGameObject(const vector<ComponentMeta *> componentMetas, GameObject &gameObject)
{
	for (ComponentMeta *componentMeta : componentMetas)
	{
		ComponentType_ componentType = componentMeta->componentType;
		if (componentType == ComponentType_Transform)
		{
			Transform *tf = gameObject.AddComponent<Transform>();
			TransformMeta *transformMeta = static_cast<TransformMeta *>(componentMeta);
			MetaToTransform(*transformMeta, *tf);
		}

		if (componentType == ComponentType_Camera)
		{
			Camera *camera = gameObject.AddComponent<Camera>();
			CameraMeta *cameraMeta = static_cast<CameraMeta *>(componentMeta);
			MetaToCamera(*cameraMeta, *camera);
			continue;
		}

		if (componentType == ComponentType_DirectLight)
		{
			DirectLight *directLight = gameObject.AddComponent<DirectLight>();
			DirectLightMeta *directLightMeta = static_cast<DirectLightMeta *>(componentMeta);
			MetaToDirectLight(*directLightMeta, *directLight);
			continue;
		}

		if (componentType == ComponentType_MeshFilter)
		{
			MeshFilter *meshFilter = gameObject.AddComponent<MeshFilter>();
			MeshFilterMeta *meshFilterMeta = static_cast<MeshFilterMeta *>(componentMeta);
			MetaToMeshFilter(*meshFilterMeta, *meshFilter);
			continue;
		}

		if (componentType == ComponentType_MeshRenderer)
		{
			MeshRenderer *meshRenderer = gameObject.AddComponent<MeshRenderer>();
			MeshRendererMeta *meshRendererMeta = static_cast<MeshRendererMeta *>(componentMeta);
			MetaToMeshRenderer(*meshRendererMeta, *meshRenderer);
			MeshFilter *meshFilter = gameObject.GetComponent<MeshFilter>();
			if (meshFilter != nullptr)
			{
				meshRenderer->GenerateRenderer(meshFilter);
			}
			continue;
		}

		if (componentType == ComponentType_SkinMeshRenderer)
		{
			SkinMeshRenderer *skinMeshRenderer = gameObject.AddComponent<SkinMeshRenderer>();
			SkinMeshRendererMeta *skinMeshRendererMeta = static_cast<SkinMeshRendererMeta *>(componentMeta);
			MetaToSkinMeshRenderer(*skinMeshRendererMeta, *skinMeshRenderer);
			continue;
		}

		cout << "RuntimeDomain::_MetaToGameObject: Unknown component type: " << componentType << endl;
	}
}

void RuntimeDomain::MetaToGameObject(const PrefabInstanceMeta &prefabInstanceMeta, GameObject &gameObject)
{
	gameObject.SetGID(prefabInstanceMeta.gameObjectMeta->gid);
	gameObject.SetGUID(prefabInstanceMeta.guid);

	PrefabMeta prefabMeta = PrefabMeta();
	GUIDToPrefabMeta(prefabInstanceMeta.guid, prefabMeta);
	MetaToGameObject(prefabMeta, gameObject);
	MetaToTransform(*prefabInstanceMeta.gameObjectMeta->transformMeta, *gameObject.transform());

	gameObject.SetName(prefabInstanceMeta.gameObjectMeta->name);
}

void RuntimeDomain::MetaToGameObject(const PrefabMeta &prefabMeta, GameObject &gameObject)
{
	gameObject.SetName(prefabMeta.name);
	_MetaToGameObject(prefabMeta.componentMetas, gameObject);
}

void RuntimeDomain::MetaToGameObject(const GameObjectMeta &gameObjectMeta, GameObject &gameObject)
{
	gameObject.SetGID(gameObjectMeta.gid);
	gameObject.SetName(gameObjectMeta.name);
	_MetaToGameObject(gameObjectMeta.componentMetas, gameObject);
}

/**
 * @brief 添加 GameObject 到 Scene
 */
inline void __AddGoToScene(GameObject *go, Scene &scene)
{
	vector<GameObject *> *allGameObjects = scene.allGameObjects;
	vector<GameObject *> *rootGameObjects = scene.rootGameObjects;
	allGameObjects->push_back(go);
	const int fatherGID_forSerialize = go->transform()->fatherGID_forSerialize;
	if (!fatherGID_forSerialize)
	{
		rootGameObjects->push_back(go);
	}
	else
	{
		for (int i = 0; i < allGameObjects->size(); i++)
		{
			GameObject *fatherGO = allGameObjects->at(i);
			if (fatherGO->GetGID() == fatherGID_forSerialize)
			{
				go->transform()->SetFather(fatherGO->transform());
				break;
			}
		}
	}
	vector<int> childrenGIDs_forSerialize = go->transform()->childrenGIDs_forSerialize;
	for (int childGID : childrenGIDs_forSerialize)
	{
		for (int i = 0; i < allGameObjects->size(); i++)
		{
			GameObject *childGO = allGameObjects->at(i);
			if (childGO->GetGID() == childGID)
			{
				go->transform()->AddChild(childGO->transform());
				break;
			}
		}
	}
}

void RuntimeDomain::MetaToScene(const SceneMeta &sceneMeta, Scene &scene)
{
	for (GameObjectMeta *meta : sceneMeta.gameObjectMetas)
	{
		GameObject *go = new GameObject();
		MetaToGameObject(*meta, *go);
		__AddGoToScene(go, scene);
	}
	for (PrefabInstanceMeta *meta : sceneMeta.prefabInstanceMetas)
	{
		GameObject *go = new GameObject();
		MetaToGameObject(*meta, *go);
		__AddGoToScene(go, scene);
	}
}

GameObject *RuntimeDomain::assetToGameObject(const string guid)
{
	PrefabMeta prefabMeta = PrefabMeta();
	GUIDToPrefabMeta(guid, prefabMeta);
	if (prefabMeta.transformMeta == nullptr)
		return nullptr;
	GameObject *gameObject = new GameObject();
	gameObject->SetGUID(guid);
	gameObject->SetName(prefabMeta.name);
	MetaToGameObject(prefabMeta, *gameObject);
	return gameObject;
}