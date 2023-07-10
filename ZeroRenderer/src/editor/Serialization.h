#pragma once
#include <sstream>
#include "Camera3D.h"
#include "Transform.h"
#include "Scene.h"
#include "MatMeta.h"
#include "ShaderMeta.h"
#include "TextureMeta.h"
using namespace std;

namespace Serialization {

	void GameObject_SerializeTo(GameObject* gameObject, stringstream& ss);
	void GameObject_SerializeTo(GameObject* gameObject, stringstream& ss);

	void Camera_SerializeTo(Camera3D* camera, stringstream& ss);
	void Camera_DeserializeFrom(Camera3D* camera, stringstream& ss);

	void Transform_SerializeTo(Transform* transform, stringstream& ss);
	void Transform_DeserializeFrom(Transform* transform, stringstream& ss);

	void Component_SerializeTo(Component* component, stringstream& ss);

	void GameObject_SerializeTo(GameObject* gameObject, stringstream& ss);
	void GameObject_DeserializeFrom(GameObject* gameObject, stringstream& ss);

	void Scene_SerializeTo(Scene* scene, const string& path);
	void Scene_DeserializeFrom(Scene* scene, const string& path);

	void MatMeta_SerializeTo(MatMeta* matMeta, const string& path);
	void MatMeta_DeserializeFrom(MatMeta* matMeta, const string& path);

	void ShaderMeta_SerializeTo(ShaderMeta* shaderMeta, const string& path);
	void ShaderMeta_DeserializeFrom(ShaderMeta* shaderMeta, const string& path);

	void TextureMeta_SerializeTo(const TextureMeta& textureMeta, const string& path);
	void TextureMeta_DeserializeFrom(TextureMeta* textureMeta, const string& path);

}

