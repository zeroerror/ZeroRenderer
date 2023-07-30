#pragma once 
#include "TransformMeta.h"
#include "EditorContext.h"
#include "RuntimeDomain.h"

class EditorDomain {

public:
	EditorContext* editorContext;
	RuntimeDomain* runtimeDomain;
	void Inject(EditorContext* editorContext, RuntimeDomain* runtimeDomain);

	void TransformMeta_SetFather(TransformMeta& child, TransformMeta& father);
	void TransformMeta_AddChild(TransformMeta& father, TransformMeta& child);
	void TransformMeta_RemoveChild(TransformMeta& father, TransformMeta& child);

private:

};