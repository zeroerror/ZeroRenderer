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

GameObject *GameObject::Find(const string &path)
{
	return _Find(path, this);
}

GameObject *GameObject::_Find(const string &path, GameObject *gameObject) const
{
	if (gameObject == nullptr)
	{
		return nullptr;
	}

	size_t end1 = path.find_last_of("/");
	if (end1 == string::npos)
	{
		return _Find(path);
	}

	size_t end2 = path.find_last_of("/");
	string name;
	if (end2 == string::npos)
	{
		name = path.substr(end1 + 1);
	}
	else
	{
		name = path.substr(end1 + 1, end2);
	}
	string nexPath = path.substr(end1 + 1);

	GameObject *nextGO = _Find(name);

	return _Find(nexPath, nextGO);
}

GameObject *GameObject::_Find(const string &name) const
{
	Transform *res = _transform->Find(name);
	if (res == nullptr)
	{
		return nullptr;
	}

	return res->gameObject;
}
