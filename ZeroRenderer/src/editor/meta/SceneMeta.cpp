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

GameObjectMeta* SceneMeta::Find(const int gid){
	for (GameObjectMeta* gameObjectMeta : gameObjectMetas) {
		if (gameObjectMeta->gid == gid) {
			return gameObjectMeta;
		}
	}
	
	return nullptr;
}
