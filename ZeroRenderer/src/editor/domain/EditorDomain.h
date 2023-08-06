#pragma once 
#include "TransformMeta.h"
#include "EditorContext.h"
#include "RuntimeDomain.h"
#include "EditorUICanvas.h"

class EditorDomain {

public:
	EditorContext* editorContext;
	RuntimeDomain* runtimeDomain;
	void Inject(EditorContext* editorContext, RuntimeDomain* runtimeDomain);

	vector<TransformMeta*>* children;
	void TransformMeta_LinkChildren(TransformMeta& src, const vector<int>* childrenGIDs);
	void TransformMeta_LinkFather(TransformMeta& src, int fatherGID);

	void EditorUICanvas_Draw(const EditorUICanvas& canvas);

	int GenerateGID();

private:
	int _gid;

};