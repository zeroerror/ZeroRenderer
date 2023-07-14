#pragma once
#include "GameObjectMeta.h"

class SceneMeta
{
public:
	std::string guid;

	vector<GameObjectMeta> gameObjectMetas;
	GameObjectMeta Find(const string& name);
};

