#pragma once

class Transform;
class Component;

#include "Camera.h"
#include "DirectLight.h"

#include <vector>
#include <sstream>
#include <functional>

using namespace std;

class GameObject
{

public:
	GameObject();
	~GameObject();

	Transform *transform() const;
	vector<Component *> GetAllComponents();

	/**
	 * @brief 根据相对路径查找GameObject
	 * @param relativePath 相对路径, 如: "root/child1/child2"
	 */
	GameObject *FindByPath(const string &relativePath);

	string GetName() const;
	void SetName(const string &name);

	int GetGID() const;
	void SetGID(const int &gid);

	string GetGUID() const;
	void SetGUID(const string &guid);

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	T *AddComponent()
	{
		// check if already exists
		Component *com = GetComponent<T>();
		if (com != nullptr)
		{
			return static_cast<T *>(com);
		}

		// add new component
		T *component = new T();
		_components.push_back(component);

		// if type is transform, set transform
		if (component->componentType == ComponentType_Transform)
		{
			this->_transform = dynamic_cast<Transform *>(component);
			// foreach component, set transform
			for (auto com : _components)
			{
				com->transform = this->_transform;
			}
		}

		// link go and transform
		component->gameObject = this;
		component->transform = this->_transform;

		return component;
	}

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	T *GetComponent()
	{
		T t = T();
		Component com = static_cast<Component>(t);
		ComponentType_ comType = com.componentType;
		if (comType == ComponentType_Transform)
		{
			return dynamic_cast<T *>(_transform);
		}

		for (auto com : _components)
		{
			if (comType == com->componentType)
			{
				return static_cast<T *>(com);
			}
		}

		return nullptr;
	}

	template <typename T, typename = enable_if_t<is_base_of<Component, T>::value>>
	void GetAllComponents(vector<T *> &components)
	{
		T t = T();
		Component c = static_cast<Component>(t);
		ComponentType_ comType = c.componentType;
		if (comType == ComponentType_Transform)
		{
			components.push_back(dynamic_cast<T *>(_transform));
		}

		for (auto com : _components)
		{
			if (com->componentType == comType)
			{
				components.push_back(dynamic_cast<T *>(com));
			}
		}
	}

	/**
	 * @brief 深度遍历(dfs: depth first search)所有子节点
	 */
	void dfsChildren(std::function<void(GameObject *)> callback);

private:
	string _name;
	int _gid;
	string _guid;
	Transform *_transform;
	vector<Component *> _components;

	/**
	 * @brief 通过名字查找GameObject
	 */
	GameObject *_FindByName(const string &name) const;
};
