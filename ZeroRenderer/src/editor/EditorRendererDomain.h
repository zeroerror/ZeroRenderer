#pragma once
#include "EditorContext.h"
#include "Model.h"
#include "ObjMetadata.h"
#include "TextureType.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class EditorRendererDomain {

	EditorContext* editorContext;

public:
	void Inject(EditorContext* ctxt);

	void DrawModel(const Model* model);
	void DrawModel(const Model* model, const Material* material);
	void BindShader(const Model* model, Shader* shader);
	void BindTexture(Texture* texture, const TextureType& textureType);

	bool TryLoadMaterialByGUID(const string& guid, Material*& mat);
	bool TryLoadMaterialByAssetPath(const string& path, Material*& mat);

	bool TryLoadModel(const string& path, Model*& model);
	void ProcessNode(aiNode* aiNode, const aiScene* aiScene, const ObjMetadata& objMeta, vector<Mesh*>* allMeshes, size_t& materialIndex);
	Mesh* ProcessMesh(aiMesh* aMesh, const aiScene* aiScene, const ObjMetadata& objMeta, size_t& materialIndex);

	void LoadDefaultScene();

private:
	void _DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light);

};

