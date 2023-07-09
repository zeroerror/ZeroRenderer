#include "Transform.h"
#include "FileHelper.h"
#include <filesystem>

Transform::Transform()
	: m_position(0.0f),
	m_rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_forward(0, 0, 1),
	m_up(0, 1, 0),
	m_right(-1, 0, 0) {
	componentType = ComponentType_Transform;
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

void Transform::SerializeTo(stringstream& ss) {
	ss << "componentType: " << componentType << std::endl;
	ss << "m_position: " << m_position.x << ' ' << m_position.y << ' ' << m_position.z << std::endl;
	ss << "m_rotation: " << m_rotation.x << ' ' << m_rotation.y << ' ' << m_rotation.z << ' ' << m_rotation.w << std::endl;
	ss << "componentEnd" << std::endl;
}

void Transform::DeserializeFrom(stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "componentEnd") {
			break;
		}
		if (key == "m_position:") {
			iss >> key;
			m_position.x = atoi(key.c_str());
			iss >> key;
			m_position.y = atoi(key.c_str());
			iss >> key;
			m_position.z = atoi(key.c_str());
		}
		else if (key == "m_rotation:") {
			iss >> key;
			m_rotation.x = atoi(key.c_str());
			iss >> key;
			m_rotation.y = atoi(key.c_str());
			iss >> key;
			m_rotation.z = atoi(key.c_str());
			iss >> key;
			m_rotation.w = atoi(key.c_str());
		}
	}
}
