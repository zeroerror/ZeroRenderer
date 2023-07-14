#pragma once

class Component;
class GameObject;
#include "GameObject.h"
#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>

using namespace std;
using namespace glm;

class Transform : public Component {
public:
	Transform();
	~Transform();

	vec3 position;
	quat rotation;
	vec3 scale;

	vec3 GetPosition() const;
	void SetPosition(const vec3& newPosition);

	quat GetRotation() const;
	void SetRotation(const quat& newRotation);

	vec3 GetForward() const;
	vec3 GetUp() const;
	vec3 GetRight() const;

private:
	vec3 m_forward;
	vec3 m_up;
	vec3 m_right;
};
