#include "EditorDomain.h"

void EditorDomain::Inject(EditorContext* editorContext, RuntimeDomain* runtimeDomain) {
	this->editorContext = editorContext;
	this->runtimeDomain = runtimeDomain;
}

void EditorDomain::TransformMeta_LinkChildren(TransformMeta& src, const vector<int>* childrenGIDs){
	SceneMeta* curSceneMeta = editorContext->currentSceneMeta;
	for (int i = 0; i < childrenGIDs->size(); i++) {
		int childGID = childrenGIDs->at(i);
		GameObjectMeta* childGO = curSceneMeta->Find(childGID);
		TransformMeta* child = childGO->transformMeta;
		src.AddChild(child);
	}
}

void EditorDomain::TransformMeta_LinkFather(TransformMeta& src, int fatherGID){
	SceneMeta* curSceneMeta = editorContext->currentSceneMeta;
	GameObjectMeta* fatherGO = curSceneMeta->Find(fatherGID);
	TransformMeta* father = fatherGO->transformMeta;
	src.SetFather(father);
}

void EditorDomain::EditorUICanvas_Draw(const EditorUICanvas& rootCanvas){
	
}
