#include "Transform.h"
#include "FileHelper.h"
#include <filesystem>

Transform::Transform()
	: position(0.0f),
	rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_forward(0, 0, 1),
	m_up(0, 1, 0),
	m_right(-1, 0, 0) {
	componentType = ComponentType_Transform;
}

Transform::~Transform() {

}

glm::vec3 Transform::GetPosition() const {
	return position;
}

void Transform::SetPosition(const glm::vec3& newPosition) {
	position = newPosition;
}

glm::quat Transform::GetRotation() const {
	return rotation;
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
	rotation = newRotation;

	// 更新前向量和上向量
	m_forward = glm::normalize(newRotation * glm::vec3(0.0f, 0.0f, 1.0f));
	m_up = glm::normalize(newRotation * glm::vec3(0.0f, 1.0f, 0.0f));
	m_right = glm::normalize(newRotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::AddChild(Transform* child) {
	_children.push_back(child);
}

void Transform::RemoveChild(Transform* child) {
	if (child == nullptr) {
		return;
	}
	for (auto it = _children.begin(); it != _children.end(); ++it) {
		if (*it == child) {
			_children.erase(it);
			break;
		}
	}
}

void Transform::SetParent(Transform* parent) {
	_parent = parent;
}

Transform* Transform::GetParent() const {
	return _parent;
}

Transform* Transform::GetChild(int index) const {
	if (index < 0 || index >= _children.size()) {
		return nullptr;
	}
	return _children[index];
}

Transform* Transform::Find(const string& path)  {
	return _Find(path, this);
}

Transform* Transform::_Find(const string& path, Transform* transform) const {
	if (transform == nullptr) {
		return nullptr;
	}

	size_t end1 = path.find_last_of("/");
	if (end1 == string::npos) {
		return  _Find(path);
	}

	size_t end2 = path.find_last_of("/");
	string name;
	if (end2 == string::npos)name = path.substr(end1 + 1);
	else name = path.substr(end1 + 1, end2);

	Transform* next = _Find(name);

	_Find(name, next);
}

Transform* Transform::_Find(const string& name) const{
	for (auto child : _children) {
		if (child->GetName() == name) {
			return child;
		}
	}

	return nullptr;
}

int Transform::GetChildCount() const {
	return _children.size();
}

string Transform::GetName() const{
	return gameObject->GetName();
}
