#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
	Transform();
	~Transform();

public:
	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& newPosition);

	glm::quat GetRotation() const;
	void SetRotation(const glm::quat& newRotation);

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;
};
