#pragma once
#include <sstream>
#include "Camera3D.h"
#include "Transform.h"
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

}

