#pragma once
#include "EditorContext.h"
#include "Model.h"
#include "Mesh.h"
#include "ObjMeta.h"
#include "PrefabMeta.h"
#include "TextureSlotCollection.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class EditorRendererDomain {

	EditorContext* editorContext;

public:
	void Inject(EditorContext* ctxt);
	void Init();
	void ProcessModel(const string& path);
	void ProcessMeshes(const aiScene* aScene, PrefabMeta& prefabMeta);

	// void DrawMesh(const MeshFilter* meshFilter,const MeshRenderer* meshRenderer);
	//void DrawModel(const Model* model);
	//void DrawModel(const Model* model, const Material* material);
	void BindShader(const Model* model, Shader* shader);

	bool TryLoadMaterialByGUID(const string& guid, Material*& mat);
	//bool TryLoadMaterialByAssetPath(const string& path, Material*& mat);

	bool TryLoadModel(const string& path, Model*& model);
	SkinMeshRenderer* LoadSkinMeshRenderer(const aiScene* aScene, PrefabMeta& prefabMeta);

	//void LoadDefaultSceneMeta();

private:
	//void _DrawModel(const Model* model, const Camera* camera, const DirectLight* light);

};

