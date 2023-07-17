#pragma once
#include "SceneMeta.h"
#include "ObjMeta.h"
#include "PrefabInstanceMeta.h"
#include "PrefabMeta.h"
#include "GameObjectMeta.h"
#include "ComponentMeta.h"
#include "CameraMeta.h"
#include "DirectLightMeta.h"
#include "MeshFilterMeta.h"
#include "MeshRendererMeta.h"
#include "SkinMeshRendererMeta.h"

#include "TextureSlotCollection.h"
#include "SkinMeshRenderer.h"
#include "RuntimeContext.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class RuntimeDomain {

	RuntimeContext* runtimeContext;

public:
	void Inject(RuntimeContext* ctxt);
	void Init();

	void ProcessModel(const string& path);
	void ProcessMeshes(const aiScene* aScene, PrefabMeta& prefabMeta);

	void BindShader(const Transform* transform, Shader* shader);
	bool TryLoadMaterialByGUID(const string& guid, Material*& mat);

	SkinMeshRenderer* LoadSkinMeshRenderer(const aiScene* aScene, PrefabMeta& prefabMeta);
	void BatchSkinMeshRenderer(SkinMeshRenderer* skinMeshRenderer);
	void BatchedDrawSkinMeshRenderer(SkinMeshRenderer* skinMeshRenderer);

	void LoadScene(const string& path);

	void DrawSkinMeshRenderer(const SkinMeshRenderer* skinMeshRenderer);
	void DrawMeshRenderer(const MeshRenderer* meshRenderer);

	bool TryLoadMaterialByAssetPath(const string& path, Material*& mat);
	bool TryLoadMesh(const string& modelGUID, const int& meshIndex, Mesh*& mesh);

	// =======================================================================
	// All meta's methods that convert to a runtime object.
	void MetaToDirectLight(const DirectLightMeta& directLightMeta, DirectLight& directLight);
	void MetaToCamera(const CameraMeta& cameraMeta, Camera& camera);
	void MetaToMeshFilter(const MeshFilterMeta& meshFilterMeta, MeshFilter& meshFilter);
	void MetaToMeshRenderer(const MeshRendererMeta& meshRendererMeta, MeshRenderer& meshRenderer);
	void MetaToSkinMeshRenderer(const SkinMeshRendererMeta& skinMeshRendererMeta, SkinMeshRenderer& skinMeshRenderer);
	void MetaToTransform(const TransformMeta& transformMeta, Transform& transform);
	void MetaToComponent(const ComponentMeta& componentMeta, Component& component);
	void MetaToPrefab(const string& guid, PrefabMeta& prefabMeta);
	void MetaToGameObject(const PrefabInstanceMeta& prefabInstanceMeta, GameObject& gameObject);
	void MetaToGameObject(const PrefabMeta& prefabMeta, GameObject& gameObject);
	void MetaToGameObject(const GameObjectMeta& gameObjectMeta, GameObject& gameObject);
	void MetaToScene(const SceneMeta& sceneMeta, Scene& scene);

private:
	void _MetaToGameObject(const TransformMeta& transformMeta, const vector<ComponentMeta*> componentMetas, GameObject& gameObject);

};

