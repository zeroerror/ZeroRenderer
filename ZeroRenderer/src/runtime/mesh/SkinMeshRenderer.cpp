#include "SkinMeshRenderer.h"

SkinMeshRenderer::SkinMeshRenderer() {
	componentType = ComponentType_SkinMeshRenderer;
    meshFilters = new vector<MeshFilter*>();
    meshRenderers = new vector<MeshRenderer*>();
}

SkinMeshRenderer::~SkinMeshRenderer() {
	delete va_batched;
	delete vb_batched;
	delete vbLayout_batched;
	delete ib_batched;

	for (auto meshFilter : *meshFilters) {
		delete meshFilter;
	}
	delete meshFilters;

	for (auto meshRenderer : *meshRenderers) {
		delete meshRenderer;
	}
	delete meshRenderers;
}