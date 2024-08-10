#pragma once
#include <string>
#include <vector>
#include "ComponentType.h"

class TransformMeta;
class ComponentMeta;

using namespace std;

class GameObjectMeta
{
public:
	GameObjectMeta();
	~GameObjectMeta();

	int gid;
	string name;

	TransformMeta *transformMeta;
	vector<ComponentMeta *> componentMetas;

	template <typename T, typename = enable_if_t<is_base_of<ComponentMeta, T>::value>>
	T *AddComponentMeta()
	{
		// check if already exists
		T *comMeta = GetComponentMeta<T>();
		if (comMeta != nullptr)
		{
			return static_cast<T *>(comMeta);
		}

		// add new component
		T *componentMeta = new T();
		componentMetas.push_back(componentMeta);

		// if type is transform, set transform
		if (componentMeta->componentType == ComponentType_Transform)
		{
			this->transformMeta = dynamic_cast<TransformMeta *>(componentMeta);
			// foreach component, set transform
			for (auto comMeta : componentMetas)
			{
				comMeta->transformMeta = this->transformMeta;
			}
		}

		// link go and transform
		componentMeta->gameObjectMeta = this;
		componentMeta->transformMeta = this->transformMeta;

		return componentMeta;
	}

	template <typename T, typename = enable_if_t<is_base_of<ComponentMeta, T>::value>>
	T *GetComponentMeta()
	{
		T t = T();
		ComponentMeta comMeta = static_cast<ComponentMeta>(t);
		ComponentType_ comType = comMeta.componentType;
		for (auto comMeta : componentMetas)
		{
			if (comType == comMeta->componentType)
			{
				return static_cast<T *>(comMeta);
			}
		}

		return nullptr;
	}
};
