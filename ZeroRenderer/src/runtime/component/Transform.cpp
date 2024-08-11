#include "Transform.h"
#include "FileHelper.h"
#include <filesystem>
#include "MeshFilter.h"
#include "MeshRenderer.h"

Transform::Transform()
	: _position(0.0f),
	  _rotation(1.0f, 0.0f, 0.0f, 0.0f),
	  _scale(1.0f, 1.0f, 1.0f),
	  _localPosition(0.0f),
	  _localRotation(1.0f, 0.0f, 0.0f, 0.0f),
	  _localScale(1.0f, 1.0f, 1.0f),
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

void Transform::UpdateLocalTRS_T()
{
	if (!_father)
	{
		// 没有父节点, 本地坐标系下的坐标、旋转、缩放等于世界坐标系下的坐标、旋转、缩放
		_localPosition = _position;
		return;
	}
	// 有父节点, 本地坐标系下的坐标、旋转、缩放等于世界坐标系下的坐标、旋转、缩放 减去 父节点的坐标、旋转、缩放
	_localPosition = _position - _father->_position;
}

void Transform::UpdateLocalTRS_R()
{
	if (!_father)
	{
		// 没有父节点, 本地坐标系下的坐标、旋转、缩放等于世界坐标系下的坐标、旋转、缩放
		_localRotation = _rotation;
		return;
	}
	// 有父节点, 本地坐标系下的坐标、旋转、缩放等于世界坐标系下的坐标、旋转、缩放 减去 父节点的坐标、旋转、缩放
	_localRotation = glm::inverse(_father->_rotation) * _rotation;
}

void Transform::UpdateLocalTRS_S()
{
	if (!_father)
	{
		// 没有父节点, 本地坐标系下的坐标、旋转、缩放等于世界坐标系下的坐标、旋转、缩放
		_localScale = _scale;
		return;
	}
	// 有父节点, 本地坐标系下的坐标、旋转、缩放等于世界坐标系下的坐标、旋转、缩放 减去 父节点的坐标、旋转、缩放
	_localScale = _scale / _father->_scale;
}

void Transform::UpdateLocalTRS()
{
	UpdateLocalTRS_T();
	UpdateLocalTRS_R();
	UpdateLocalTRS_S();
}

glm::vec3 Transform::GetPosition() const
{
	return _position;
}

void Transform::SetPosition(const glm::vec3 &newPosition)
{
	bool hasChildren = _children->size() > 0;
	if (hasChildren)
	{
		// 更新所有子节点世界坐标
		glm::vec3 offset = newPosition - _position;
		this->DFSChildren([offset](Transform *trans)
						  { trans->_position += offset; });
	}
	if (_father)
	{
		// 更新父节点的子节点列表
		const int index = _father->GetIndex(gameObject->GetGID());
		if (index != -1)
		{
			_father->childrenGIDs_forSerialize[index] = gameObject->GetGID();
		}
	}

	// 更新自身
	_position = newPosition;
}

glm::quat Transform::GetRotation() const
{
	return _rotation;
}

void Transform::SetRotation(const glm::quat &newRotation)
{
	bool hasChildren = _children->size() > 0;
	if (hasChildren)
	{
		glm::quat offset = glm::inverse(_rotation) * newRotation;
		// 更新所有子节点世界坐标系下的 旋转、方向向量、坐标
		this->DFSChildren([offset, this](Transform *child) { // 旋转
			child->_rotation = offset * child->_rotation;
			// 方向向量
			child->m_forward = glm::normalize(child->_rotation * glm::vec3(0.0f, 0.0f, 1.0f));
			child->m_up = glm::normalize(child->_rotation * glm::vec3(0.0f, 1.0f, 0.0f));
			child->m_right = glm::normalize(child->_rotation * glm::vec3(1.0f, 0.0f, 0.0f));
			// 坐标
			child->_position = _position + offset * (child->_position - _position);
		});
	}
	// 更新自身
	_rotation = newRotation;
	m_forward = glm::normalize(newRotation * glm::vec3(0.0f, 0.0f, 1.0f));
	m_up = glm::normalize(newRotation * glm::vec3(0.0f, 1.0f, 0.0f));
	m_right = glm::normalize(newRotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::SetRotation(const glm::vec3 &eulerAngles)
{
	auto radian = glm::radians(eulerAngles);
	bool hasChildren = _children->size() > 0;
	if (hasChildren)
	{
		glm::quat offset = glm::inverse(_rotation) * glm::quat(radian);
		// 更新所有子节点世界坐标系下的 旋转、方向向量、坐标
		this->DFSChildren([offset, this](Transform *child) { // 旋转
			child->_rotation = offset * child->_rotation;
			// 方向向量
			child->m_forward = glm::normalize(child->_rotation * glm::vec3(0.0f, 0.0f, 1.0f));
			child->m_up = glm::normalize(child->_rotation * glm::vec3(0.0f, 1.0f, 0.0f));
			child->m_right = glm::normalize(child->_rotation * glm::vec3(1.0f, 0.0f, 0.0f));
			// 坐标
			child->_position = _position + offset * (child->_position - _position);
		});
	}
	// 更新自身
	_rotation = glm::quat(radian);
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
	bool hasChildren = _children->size() > 0;
	if (hasChildren)
	{
		glm::vec3 offset = scale / _scale;
		// 更新所有子节点世界坐标系下的缩放
		this->DFSChildren([offset](Transform *child)
						  { child->_scale *= offset; });
	}
	// 更新自身
	this->_scale = scale;
}

vec3 Transform::GetLocalPosition() const
{
	return _localPosition;
}

void Transform::SetLocalPosition(const vec3 &newPosition)
{
	_localPosition = newPosition;
}

quat Transform::GetLocalRotation() const
{
	return _localRotation;
}

void Transform::SetLocalRotation(const quat &newRotation)
{
	_localRotation = newRotation;
}

void Transform::SetLocalRotation(const vec3 &eulerAngles)
{
	_localRotation = glm::quat(glm::radians(eulerAngles));
}

vec3 Transform::GetLocalScale() const
{
	return _localScale;
}

void Transform::SetLocalScale(const vec3 &scale)
{
	_localScale = scale;
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
	child->fatherGID_forSerialize = gameObject->GetGID();
	this->_children->push_back(child);
	childrenGIDs_forSerialize.push_back(child->gameObject->GetGID());
	UpdateLocalTRS();
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
	UpdateLocalTRS();
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

void Transform::DFSChildren(std::function<void(Transform *)> callback)
{
	std::function<void(Transform *)> func = [callback](Transform *trans)
	{
		callback(trans);
		trans->DFSChildren(callback);
	};
	this->ForEachChild(func);
}
