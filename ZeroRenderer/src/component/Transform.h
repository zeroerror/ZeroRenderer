#pragma once

class Component;
class GameObject;
#include "GameObject.h"
#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>

using namespace std;

class Transform : public Component {
public:
	Transform();
	~Transform();

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& newPosition);

	glm::quat GetRotation() const;
	void SetRotation(const glm::quat& newRotation);

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	void SerializeTo(stringstream& ss);
	void DeserializeFrom(stringstream& ss);

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;
};
