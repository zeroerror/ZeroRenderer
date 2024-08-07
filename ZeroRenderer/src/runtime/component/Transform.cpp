#include "Transform.h"
#include "FileHelper.h"
#include <filesystem>
#include "MeshFilter.h"
#include "MeshRenderer.h"

Transform::Transform()
	: _position(0.0f),
	  _rotation(1.0f, 0.0f, 0.0f, 0.0f),
	  _scale(1.0f, 1.0f, 1.0f),
	  m_forward(0, 0, 1),
	  m_up(0, 1, 0),
	  m_right(-1, 0, 0)
{
	componentType = ComponentType_Transform;

	_children = new vector<Transform *>();
	childrenGIDs_forSerialize = vector<int>();
}

Transform::~Transform()
{
	delete _children;
}

glm::vec3 Transform::GetPosition() const
{
	return _position;
}

void Transform::SetPosition(const glm::vec3 &newPosition)
{
	_position = newPosition;
}

glm::quat Transform::GetRotation() const
{
	return _rotation;
}

void Transform::SetRotation(const glm::quat &newRotation)
{
	_rotation = newRotation;
	m_forward = glm::normalize(newRotation * glm::vec3(0.0f, 0.0f, 1.0f));
	m_up = glm::normalize(newRotation * glm::vec3(0.0f, 1.0f, 0.0f));
	m_right = glm::normalize(newRotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::SetRotation(const glm::vec3 &eulerAngles)
{
	_rotation = glm::quat(glm::radians(eulerAngles));
	m_forward = glm::normalize(_rotation * glm::vec3(0.0f, 0.0f, 1.0f));
	m_up = glm::normalize(_rotation * glm::vec3(0.0f, 1.0f, 0.0f));
	m_right = glm::normalize(_rotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

vec3 Transform::GetScale() const
{
	return this->_scale;
}

void Transform::SetScale(const vec3 &scale)
{
	this->_scale = scale;
}

glm::vec3 Transform::GetForward() const
{
	return m_forward;
}

glm::vec3 Transform::GetUp() const
{
	return m_up;
}

glm::vec3 Transform::GetRight() const
{
	return m_right;
}

void Transform::SetFather(Transform *father)
{
	this->_father = father;
	father->_children->push_back(this);
}

void Transform::AddChild(Transform *child)
{
	this->_children->push_back(child);
	child->_father = this;
}

void Transform::RemoveChild(const Transform &child)
{
	for (int i = 0; i < _children->size(); i++)
	{
		if (_children->at(i)->gameObject->GetGID() == child.gameObject->GetGID())
		{
			_children->erase(_children->begin() + i);
			break;
		}
	}
}

Transform *Transform::GetFather() const { return _father; }

Transform *Transform::GetChild(int index) const
{
	if (index < 0 || index >= _children->size())
	{
		return nullptr;
	}
	return (*_children)[index];
}

void Transform::ForEachChild(void (*func)(Transform *transform))
{
	for (auto child : *_children)
	{
		func(child);
	}
}

Transform *Transform::Find(const string &path)
{
	return _Find(path, this);
}

Transform *Transform::_Find(const string &path, Transform *transform) const
{
	if (transform == nullptr)
		return nullptr;

	size_t end1 = path.find_last_of("/");
	if (end1 == string::npos)
		return _Find(path);

	size_t end2 = path.find_last_of("/");
	string name;
	if (end2 == string::npos)
		name = path.substr(end1 + 1);
	else
		name = path.substr(end1 + 1, end2);

	Transform *next = _Find(name);
	_Find(name, next);
}

Transform *Transform::_Find(const string &name) const
{
	for (auto child : *_children)
	{
		if (child->GetName() == name)
		{
			return child;
		}
	}

	return nullptr;
}

int Transform::GetChildCount() const
{
	return _children->size();
}

string Transform::GetName() const
{
	return gameObject->GetName();
}
