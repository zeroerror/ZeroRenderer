#include "EditorDomain.h"

void EditorDomain::Inject(EditorContext* editorContext, RuntimeDomain* runtimeDomain) {
	this->editorContext = editorContext;
	this->runtimeDomain = runtimeDomain;
}

void EditorDomain::TransformMeta_SetFather(TransformMeta& child, TransformMeta& father) {

}

void EditorDomain::TransformMeta_AddChild(TransformMeta& father, TransformMeta& child) {

}

void EditorDomain::TransformMeta_RemoveChild(TransformMeta& father, TransformMeta& child) {

}