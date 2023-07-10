#pragma once
#include "EditorContext.h"
#include "Model.h"
#include "ObjMeta.h"
#include "TextureSlotCollection.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class EditorRendererDomain {

	EditorContext* editorContext;

public:
	void Inject(EditorContext* ctxt);

	void DrawModel(const Model* model);
	void DrawModel(const Model* model, const Material* material);
	void BindShader(const Model* model, Shader* shader);

	bool TryLoadMaterialByGUID(const string& guid, Material*& mat);
	bool TryLoadMaterialByAssetPath(const string& path, Material*& mat);

	bool TryLoadModel(const string& path, Model*& model);
	void ProcessNode(aiNode* aiNode, const aiScene* aiScene, const ObjMeta& objMeta, vector<Mesh*>* allMeshes, size_t& materialIndex);
	Mesh* ProcessMesh(aiMesh* aMesh, const aiScene* aiScene, const ObjMeta& objMeta, size_t& materialIndex);

	void LoadDefaultScene();

private:
	void _DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light);

};

