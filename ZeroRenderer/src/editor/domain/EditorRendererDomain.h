#pragma once
#include "EditorContext.h"
#include "ObjMeta.h"
#include "PrefabMeta.h"
#include "TextureSlotCollection.h"
#include "SkinMeshRenderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class EditorRendererDomain {

	EditorContext* editorContext;

public:
	void Inject(EditorContext* ctxt);

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

	// void DrawMesh(const MeshFilter* meshFilter,const MeshRenderer* meshRenderer);
	//void DrawModel(const Model* model);
	//void DrawModel(const Model* model, const Material* material);
	bool TryLoadMaterialByAssetPath(const string& path, Material*& mat);

private:
	//void _DrawModel(const Model* model, const Camera* camera, const DirectLight* light);

};

