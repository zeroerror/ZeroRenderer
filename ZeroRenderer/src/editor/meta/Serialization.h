#pragma once
#include <sstream>
#include "CameraMeta.h"
#include "ComponentMeta.h"
#include "GameObjectMeta.h"
#include "SkinMeshRendererMeta.h"
#include "MaterialMeta.h"
#include "ObjMeta.h"
#include "PrefabMeta.h"
#include "TransformMeta.h"
#include "SceneMeta.h"
#include "ShaderMeta.h"
#include "TextureMeta.h"
using namespace std;

namespace Serialization {

	void SceneMeta_SerializeTo(const SceneMeta& sceneMeta, const string& path);
	void SceneMeta_DeserializeFrom(SceneMeta* sceneMeta, const string& path);

	void PrefabMeta_SerializeTo(PrefabMeta& prefabMeta, const string& path);
	void PrefabMeta_DeserializeFrom(PrefabMeta* prefabMeta, const string& path);

	void MaterialMeta_SerializeTo(const MaterialMeta& matMeta, const string& path);
	void MaterialMeta_DeserializeFrom(MaterialMeta* matMeta, const string& path);

	void ShaderMeta_SerializeTo(ShaderMeta* shaderMeta, const string& path);
	void ShaderMeta_DeserializeFrom(ShaderMeta* shaderMeta, const string& path);

	void TextureMeta_SerializeTo(const TextureMeta& textureMeta, const string& path);
	void TextureMeta_DeserializeFrom(TextureMeta* textureMeta, const string& path);

	void ObjMeta_SerializeTo(const ObjMeta& objMeta, const string& path);
	void ObjMeta_DeserializeFrom(ObjMeta* objMeta, const string& path);

	void GameObjectMeta_SerializeTo(const GameObjectMeta& gameObjectMeta, stringstream& ss);
	void GameObjectMeta_DeserializeFrom(GameObjectMeta* gameObjectMeta, stringstream& ss);

	void ComponentMeta_SerializeTo(const ComponentMeta& componentMeta, stringstream& ss);

	void TransformMeta_SerializeTo(const TransformMeta& transformMeta, stringstream& ss);
	void TransformMeta_DeserializeFrom(TransformMeta* transformMeta, stringstream& ss);

	void CameraMeta_SerializeTo(const CameraMeta& cameraMeta, stringstream& ss);
	void CameraMeta_DeserializeFrom(CameraMeta* cameraMeta, stringstream& ss);

	void SkinMeshRendererMeta_SerializeTo(const SkinMeshRendererMeta& skinMeshRendererMeta, stringstream& ss);
	void SkinMeshRendererMeta_DeserializeFrom(SkinMeshRendererMeta* skinMeshRendererMeta, stringstream& ss);

	void MeshFilterMeta_SerializeTo(const MeshFilterMeta& meshFilterMeta, stringstream& ss);
	void MeshFilterMeta_DeserializeFrom(MeshFilterMeta* meshFilterMeta, stringstream& ss);

	void MeshRendererMeta_SerializeTo(const MeshRendererMeta& meshRendererMeta, stringstream& ss);
	void MeshRendererMeta_DeserializeFrom(MeshRendererMeta* meshRendererMeta, stringstream& ss);

}

