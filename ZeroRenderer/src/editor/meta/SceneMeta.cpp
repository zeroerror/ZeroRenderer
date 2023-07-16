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

void SceneMeta::ToScene(Scene& scene) {
	for (GameObjectMeta* gameObjectMeta : gameObjectMetas) {
		GameObject* go = new GameObject();
		gameObjectMeta->ToGameObject(*go);
		scene.gameObjects.push_back(go);
	}
}
