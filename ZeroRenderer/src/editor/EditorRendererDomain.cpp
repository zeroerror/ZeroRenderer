#include "EditorRendererDomain.h"

void EditorRendererDomain::Inject(EditorContext* ctxt) {
	this->editorContext = ctxt;
}

void EditorRendererDomain::RenderModel(const Model* model, const Camera3D* camera, const DirectLight* light) {
}