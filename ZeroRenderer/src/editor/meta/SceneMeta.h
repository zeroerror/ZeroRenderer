#pragma once
#include "GameObjectMeta.h"
#include "PrefabInstanceMeta.h"
#include "Scene.h"

class SceneMeta
{
public:
	SceneMeta();
	~SceneMeta();

	std::string guid;

	vector<GameObjectMeta*> gameObjectMetas;
	vector<PrefabInstanceMeta*> prefabInstanceMetas;
};
