#pragma once
#include "EditorContext.h"
#include "Model.h"

class EditorRendererDomain {

	EditorContext* editorContext;

public:
	void Inject(EditorContext* ctxt);

	void DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light);
	void DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light, const Material* material);
	void _DrawModel(const Model* model, const Camera3D* camera, const DirectLight* light);
	void BindMaterial(const Model* model, const Material* material);

	bool TryLoadMaterialByGUID(const string& guid, Material*& mat);
	bool TryLoadMaterialByAssetPath(const string& path, Material*& mat);

	void LoadDefaultScene();
};

