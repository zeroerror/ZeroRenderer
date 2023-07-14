#include "SkinMeshRendererMeta.h"

SkinMeshRendererMeta::SkinMeshRendererMeta() {
	componentType = ComponentType_SkinMeshRenderer;
	meshFilterMetas = vector<MeshFilterMeta*>();
	meshRendererMetas = vector<MeshRendererMeta*>();
}