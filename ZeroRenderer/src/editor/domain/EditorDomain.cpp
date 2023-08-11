#include "EditorDomain.h"

void EditorDomain::Inject(EditorContext* editorContext, RuntimeDomain* runtimeDomain) {
	this->editorContext = editorContext;
	this->runtimeDomain = runtimeDomain;
}

void EditorDomain::TransformMeta_LinkChildren(TransformMeta& src, const vector<int>* childrenGIDs) {
	SceneMeta* curSceneMeta = editorContext->currentSceneMeta;
	for (int i = 0; i < childrenGIDs->size(); i++) {
		int childGID = childrenGIDs->at(i);
		GameObjectMeta* childGO = curSceneMeta->Find(childGID);
		TransformMeta* child = childGO->transformMeta;
		src.AddChild(child);
	}
}

void EditorDomain::TransformMeta_LinkFather(TransformMeta& src, int fatherGID) {
	SceneMeta* curSceneMeta = editorContext->currentSceneMeta;
	GameObjectMeta* fatherGO = curSceneMeta->Find(fatherGID);
	TransformMeta* father = fatherGO->transformMeta;
	src.SetFather(father);
}

void EditorDomain::SceneView_FocusOn(const vec3& pivot) {
	auto sceneView = editorContext->sceneView;
	auto fsmCom = sceneView->FSMCom();
	auto sceneViewCamera = sceneView->SceneViewCamera();
	auto forward = sceneViewCamera->transform->GetForward();
	vec3 endPos = pivot - forward * sceneView->cameraDistance;
	vec3 startPos = sceneViewCamera->transform->GetPosition();
	fsmCom->Enter_Focusing(startPos, endPos, 0.5f);
}