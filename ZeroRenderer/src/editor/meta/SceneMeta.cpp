#include "SceneMeta.h"

SceneMeta::SceneMeta() {
	gameObjectMetas = vector<GameObjectMeta*>();
	prefabInstanceMetas = vector<PrefabInstanceMeta*>();
}

SceneMeta::~SceneMeta() {
	for (GameObjectMeta* gameObjectMeta : gameObjectMetas) {
		delete gameObjectMeta;
	}
	for (PrefabInstanceMeta* prefabInstanceMeta : prefabInstanceMetas) {
		delete prefabInstanceMeta;
	}
}
