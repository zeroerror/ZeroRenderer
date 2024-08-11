#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "Camera.h"

GameObject::GameObject()
{
	_components = vector<Component *>();
	_transform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
}

Transform *GameObject::transform() const
{
	return _transform;
}

vector<Component *> GameObject::GetAllComponents()
{
	return _components;
}

void GameObject::SetName(const string &name) { _name = name; }
string GameObject::GetName() const { return _name; }

int GameObject::GetGID() const { return _gid; }
void GameObject::SetGID(const int &gid) { _gid = gid; }

string GameObject::GetGUID() const { return _guid; }
void GameObject::SetGUID(const string &guid) { _guid = guid; }

GameObject *GameObject::FindByPath(const string &path)
{
	Transform *res = _transform->FindByPath(path);
	if (res == nullptr)
	{
		return nullptr;
	}
	return res->gameObject;
}

GameObject *GameObject::_FindByName(const string &name) const
{
	Transform *res = _transform->FindByName(name);
	if (res == nullptr)
	{
		return nullptr;
	}
	return res->gameObject;
}

/**
 * @brief 深度遍历(dfs: depth first search)所有子节点
 */
void GameObject::DFSChildren(std::function<void(GameObject *)> callback)
{
	std::function<void(Transform *)> func = [callback](Transform *trans)
	{
		GameObject *nextGO = trans->gameObject;
		callback(nextGO);
		nextGO->DFSChildren(callback);
	};
	_transform->ForEachChild(func);
}