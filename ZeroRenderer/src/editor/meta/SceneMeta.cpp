#include "SceneMeta.h"

SceneMeta::SceneMeta() {
	gameObjectMetas = vector<GameObjectMeta*>();
}

SceneMeta::~SceneMeta() {
	for (GameObjectMeta* gameObjectMeta : gameObjectMetas) {
		delete gameObjectMeta;
	}
}

void SceneMeta::ToScene(Scene& scene){
	for (GameObjectMeta* gameObjectMeta : gameObjectMetas) {
		GameObject* go = new GameObject();
		gameObjectMeta->ToGameObject(*go);
		scene.gameObjects.push_back(go);
	}
}
