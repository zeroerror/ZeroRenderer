#pragma once
#include "TransformMeta.h"
#include "ComponentMeta.h"
#include "GameObjectMeta.h"
#include <vector>

using namespace std;

class GameObjectMeta
{
public:
	GameObjectMeta() {}
	~GameObjectMeta(){
		for (auto comMeta : componentMetas) {
			delete comMeta;
		}
	}
	
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
		for (auto comMeta : componentMetas) {
			if (typeid(comMeta) == typeid(T)) {
				return static_cast<T*>(comMeta);
			}
		}

		return nullptr;
	}
};

