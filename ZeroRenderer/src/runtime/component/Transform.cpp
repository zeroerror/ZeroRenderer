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

	_children = new vector<Transform*>();
}

Transform::~Transform() {
	delete _children;
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
	m_forward = glm::normalize(newRotation * glm::vec3(0.0f, 0.0f, 1.0f));
	m_up = glm::normalize(newRotation * glm::vec3(0.0f, 1.0f, 0.0f));
	m_right = glm::normalize(newRotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::SetFather(Transform* father) {
	this->_father = father;
	father->_children->push_back(this);
}

void Transform::AddChild(Transform* child) {
	this->_children->push_back(child);
	child->_father = this;
}

void Transform::RemoveChild(const Transform& child) {
	for (int i = 0; i < _children->size(); i++) {
		if (_children->at(i)->gameObject->GetGID() == child.gameObject->GetGID()) {
			_children->erase(_children->begin() + i);
			break;
		}
	}
}

Transform* Transform::GetFather() const { return _father; }

Transform* Transform::GetChild(int index) const {
	if (index < 0 || index >= _children->size()) { return nullptr; }
	return (*_children)[index];
}

Transform* Transform::Find(const string& path) {
	return _Find(path, this);
}

Transform* Transform::_Find(const string& path, Transform* transform) const {
	if (transform == nullptr) return nullptr;

	size_t end1 = path.find_last_of("/");
	if (end1 == string::npos) return _Find(path);

	size_t end2 = path.find_last_of("/");
	string name;
	if (end2 == string::npos)name = path.substr(end1 + 1);
	else name = path.substr(end1 + 1, end2);

	Transform* next = _Find(name);
	_Find(name, next);
}

Transform* Transform::_Find(const string& name) const {
	for (auto child : *_children) {
		if (child->GetName() == name) {
			return child;
		}
	}

	return nullptr;
}

int Transform::GetChildCount() const {
	return _children->size();
}

string Transform::GetName() const {
	return gameObject->GetName();
}
