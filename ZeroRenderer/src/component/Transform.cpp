#include "Transform.h"

Transform::Transform()
	: m_position(0.0f),
	m_rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_forward(0, 0, 1),
	m_up(0, 1, 0),
	m_right(-1, 0, 0) {

}

Transform::~Transform() {

}

glm::vec3 Transform::GetPosition() const {
	return m_position;
}

void Transform::SetPosition(const glm::vec3& newPosition) {
	m_position = newPosition;
}

glm::quat Transform::GetRotation() const {
	return m_rotation;
}

glm::vec3 Transform::GetForward() const {
	return m_forward;
}

glm::vec3 Transform::GetUp() const {
	return m_up;
}

glm::vec3 Transform::GetRight() const {
	return m_right;
}

void Transform::SetRotation(const glm::quat& newRotation) {
	m_rotation = newRotation;

	// 更新前向量和上向量
	m_forward = glm::normalize(newRotation * glm::vec3(0.0f, 0.0f, 1.0f));
	m_up = glm::normalize(newRotation * glm::vec3(0.0f, 1.0f, 0.0f));
	m_right = glm::normalize(newRotation * glm::vec3(1.0f, 0.0f, 0.0f));
}
