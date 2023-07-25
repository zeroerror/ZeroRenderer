#include "SceneRepo.h"
#include "EditorDatabase.h"
#include "Scene.h"
#include <iostream>

SceneRepo::SceneRepo() {
	allScenes = unordered_map<string, Scene*>();
}

SceneRepo::~SceneRepo() {
	cout << "SceneRepo::~SceneRepo() " << endl;
	for (auto& scene : allScenes) {
		scene.second->~Scene();
	}
}

bool SceneRepo::TryAddScene(const string& guid, Scene*& scene) {
	if (!EditorDatabase::GUIDExist(guid)) {
		return false;
	}

	allScenes.insert(pair<string, Scene*>(guid, scene));

	return true;
}

bool SceneRepo::TryGetScene(const string& guid, Scene*& scene) {
	unordered_map<string, Scene*>::iterator it = allScenes.find(guid);
	if (it == allScenes.end()) {
		return false;
	}

	scene = it->second;
	return true;
}