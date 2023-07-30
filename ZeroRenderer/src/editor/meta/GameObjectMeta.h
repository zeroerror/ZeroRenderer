#pragma once
#include "GameObjectMeta.h"

#include "GameObject.h"
#include "TransformMeta.h"
#include "ComponentMeta.h"

#include <vector>

using namespace std;

class GameObjectMeta
{
public:
	GameObjectMeta();
	~GameObjectMeta();

	string name;

	TransformMeta transformMeta;
	vector<ComponentMeta*> componentMetas;

	template <typename T, typename = enable_if_t<is_base_of<ComponentMeta, T>::value>>
	T* AddComponentMeta() {
		T* comMeta = new T();
		componentMetas.push_back(comMeta);
		return comMeta;
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

