#pragma once
#include <sstream>
#include "CameraMeta.h"
#include "DirectLightMeta.h"
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

	void TransformMeta_SerializeTo(const TransformMeta& transformMeta, stringstream& ss);
	void TransformMeta_DeserializeFrom(TransformMeta* transformMeta, stringstream& ss);

	void CameraMeta_SerializeTo(const CameraMeta& cameraMeta, stringstream& ss);
	void CameraMeta_DeserializeFrom(CameraMeta* cameraMeta, stringstream& ss);

	void DirectLightMeta_SerializeTo(const DirectLightMeta& DirectLightMeta, stringstream& ss);
	void DirectLightMeta_DeserializeFrom(DirectLightMeta* DirectLightMeta, stringstream& ss);

	void MeshFilterMeta_SerializeTo(const MeshFilterMeta& meshFilterMeta, stringstream& ss);
	void MeshFilterMeta_DeserializeFrom(MeshFilterMeta* meshFilterMeta, stringstream& ss);

	void MeshRendererMeta_SerializeTo(const MeshRendererMeta& meshRendererMeta, stringstream& ss);
	void MeshRendererMeta_DeserializeFrom(MeshRendererMeta* meshRendererMeta, stringstream& ss);

	void SkinMeshRendererMeta_SerializeTo(const SkinMeshRendererMeta& skinMeshRendererMeta, stringstream& ss);
	void SkinMeshRendererMeta_DeserializeFrom(SkinMeshRendererMeta* skinMeshRendererMeta, stringstream& ss);

	void ComponentMeta_SerializeTo(const ComponentMeta& componentMeta, stringstream& ss);

	void PrefabInstanceMeta_SerializeTo(PrefabInstanceMeta& prefabInstanceMeta, stringstream& ss);
	void PrefabInstanceMeta_DeserializeFrom(PrefabInstanceMeta* prefabInstanceMeta, stringstream& ss);

	void GameObjectMeta_SerializeTo(const GameObjectMeta& gameObjectMeta, stringstream& ss);
	void GameObjectMeta_DeserializeFrom(GameObjectMeta* gameObjectMeta, stringstream& ss);

	/**
	 * @param path : the path of the file to be serialized. ps: the path must be a file path and the file's suffix must be .scene
	 */
	void SceneMeta_SerializeTo(const SceneMeta& sceneMeta, const string& path);
	/**
	 * @param path : the path of the file to be deserialized. ps: the path must be a file path and the file's suffix must be .scene
	 */
	void SceneMeta_DeserializeFrom(SceneMeta* sceneMeta, const string& path);
	/**
	 * @param path : the path of the file to be serialized. ps: the path must be a file path and the file's suffix must be .prefab
	 */
	void PrefabMeta_SerializeTo(const PrefabMeta& prefabMeta, const string& path);
	/**
	 * @param path : the path of the file to be deserialized. ps: the path must be a file path and the file's suffix must be .prefab
	 */
	void PrefabMeta_DeserializeFrom(PrefabMeta& prefabMeta, const string& path);
	/**
	* @param path : the path of the file to be serialized. ps: the path must be a file path and the file's suffix must be .mat
	*/
	void MaterialMeta_SerializeTo(const MaterialMeta& matMeta, const string& path);
	/**
	 * @param path : the path of the file to be deserialized. ps: the path must be a file path and the file's suffix must be .mat
	 */
	void MaterialMeta_DeserializeFrom(MaterialMeta* matMeta, const string& path);
	/**
	* @param path : the path of the file to be serialized. ps: the path must be a file path and the file's suffix must be .shader
	*/
	void ShaderMeta_SerializeTo(const ShaderMeta& shaderMeta, const string& path);
	/**
	* @param path : the path of the file to be deserialized. ps: the path must be a file path and the file's suffix must be .shader
	*/
	void ShaderMeta_DeserializeFrom(ShaderMeta* shaderMeta, const string& path);
	/**
	 * @param path : the path of the file to be serialized. ps: the path must be a file path and the file's suffix must be like .png .jpg, etc.
	 */
	void TextureMeta_SerializeTo(const TextureMeta& textureMeta, const string& path);
	/**
	 * @param path : the path of the file to be deserialized. ps: the path must be a file path and the file's suffix must be like .png .jpg, etc.
	 */
	void TextureMeta_DeserializeFrom(TextureMeta* textureMeta, const string& path);
	/**
	 * @param path : the path of the file to be serialized. ps: the path must be a file path and the file's suffix must be .obj
	 */
	void ObjMeta_SerializeTo(const ObjMeta& objMeta, const string& path);
	/**
	 * @param path : the path of the file to be deserialized. ps: the path must be a file path and the file's suffix must be .obj
	 */
	void ObjMeta_DeserializeFrom(ObjMeta* objMeta, const string& path);

}

