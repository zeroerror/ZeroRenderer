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
	if (father)
	{
		// 新的父节点不为空, 添加到新的父节点
		father->AddChild(this);
	}
	else if (this->_father)
	{
		// 新的父节点为空, 从当前父节点移除
		this->_father->RemoveChild(this);
	}
}

void Transform::AddChild(Transform *child)
{
	if (!child)
	{
		return;
	}
	// 移除原来的父节点
	Transform *oldFather = child->_father;
	if (oldFather)
	{
		oldFather->RemoveChild(child);
	}
	// 设置新的父节点
	child->_father = this;
	this->_children->push_back(child);
	childrenGIDs_forSerialize.push_back(child->gameObject->GetGID());
}

void Transform::RemoveChild(Transform *trans)
{
	if (!trans)
		return;
	const int matchGID = trans->gameObject->GetGID();
	const int index = GetIndex(matchGID);
	if (index == -1)
	{
		return;
	}
	// 移除子节点
	_children->erase(_children->begin() + index);
	childrenGIDs_forSerialize.erase(childrenGIDs_forSerialize.begin() + index);
	trans->_father = nullptr;
	trans->fatherGID_forSerialize = 0;
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

void Transform::ForEachChild(std::function<void(Transform *)> func) const
{
	for (auto child : *_children)
	{
		func(child);
	}
}

Transform *Transform::FindByPath(const string &relativePath) const
{
	const string norPath = FileHelper::NormalizedPath(relativePath);
	if (norPath == "")
	{
		return nullptr;
	}
	auto first = norPath.find_first_of('/');
	auto name = norPath.substr(0, first);
	auto child = FindByName(name);
	if (child == nullptr)
	{
		return nullptr;
	}
	if (first == string::npos)
	{
		return child;
	}
	return child->FindByPath(norPath.substr(first + 1));
}

Transform *Transform::FindByGid(const int gid) const
{
	for (auto child : *_children)
	{
		if (child->gameObject->GetGID() == gid)
		{
			return child;
		}
	}
	return nullptr;
}

int Transform::GetIndex(const int gid) const
{
	for (int i = 0; i < _children->size(); i++)
	{
		if ((*_children)[i]->gameObject->GetGID() == gid)
		{
			return i;
		}
	}
	return -1;
}

Transform *Transform::FindByName(const string &name) const
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
