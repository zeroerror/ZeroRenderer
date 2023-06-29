#pragma once
#include "EditorContext.h"
#include "Model.h"

class EditorRendererDomain {
	
	EditorContext* editorContext;

public:
	void Inject(EditorContext* ctxt);

	void RenderModel(const Model* model,const Camera3D* camera, const DirectLight* light);

};

