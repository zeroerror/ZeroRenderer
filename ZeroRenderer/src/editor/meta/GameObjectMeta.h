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

	TransformMeta* transformMeta;
	vector<ComponentMeta*> componentMetas;

	template <typename T, typename = enable_if_t<is_base_of<ComponentMeta, T>::value>>
	T* AddComponentMeta() {
		T* componentMeta = new T();
		componentMeta->gameObjectMeta = this;
		componentMeta->transformMeta = this->transformMeta;
		componentMetas.push_back(componentMeta);
		return componentMeta;
	}

	template <typename T, typename = enable_if_t<is_base_of<ComponentMeta, T>::value>>
	T* GetComponentMeta() {
		T t = T();
		ComponentMeta comMeta = static_cast<ComponentMeta>(t);
		ComponentType_ comType = comMeta.componentType;
		for (auto comMeta : componentMetas) {
			if (comType == comMeta->componentType) {
				return static_cast<T*>(comMeta);
			}
		}

		return nullptr;
	}

};

