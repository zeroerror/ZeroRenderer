#pragma once
#include "GameObjectMeta.h"
#include "Scene.h"

class SceneMeta
{
public:
	SceneMeta();
	~SceneMeta();

	std::string guid;

	vector<GameObjectMeta*> gameObjectMetas;

	void ToScene(Scene& scene);
};
