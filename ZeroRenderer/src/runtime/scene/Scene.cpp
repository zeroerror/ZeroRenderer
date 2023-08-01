#include "Scene.h"
#include "FileHelper.h"
#include "Serialization.h"
using namespace Serialization;

Scene::Scene() {}

Scene::~Scene() {
	for (auto go : gameObjects) {
		delete go;
	}
}

GameObject* Scene::Find(const string& path) {
	string normalizedPath = FileHelper::NormalizedPath(path);
	size_t end1 = normalizedPath.find_last_of("/");
	if (end1 == string::npos) {
		for (auto go : gameObjects) {
			if (go->GetName() == normalizedPath) {
				return go;
			}
		}

		return nullptr;
	}

	normalizedPath = normalizedPath.substr(end1 + 1);
	for (auto go : gameObjects) {
		GameObject* res = go->Find(normalizedPath);
		if (res != nullptr) {
			return res;
		}
	}

	return nullptr;
}

GameObject* Scene::Find(const int& gid) {
	for (auto go : gameObjects) {
		int id = go->GetGID();
		if (id == gid) {
			return go;
		}
	}

	return nullptr;
}
