#pragma once
#include <string>
#include "GameObjectMeta.h"
#include "TransformMeta.h"

using namespace std;

class PrefabInstanceMeta {

public:
	PrefabInstanceMeta();
	~PrefabInstanceMeta();

	string guid;
	GameObjectMeta* gameObjectMeta;
};

