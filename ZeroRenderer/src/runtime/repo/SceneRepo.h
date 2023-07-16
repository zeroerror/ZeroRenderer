#pragma once
#include <string>
#include <Scene.h>
#include <unordered_map>

using namespace std;

class SceneRepo {

public:
	SceneRepo();
	~SceneRepo();

	bool TryAddScene(const string& sceneGUID, Scene*& scene);
	bool TryGetScene(const string& sceneGUID, Scene*& scene);

private:
	unordered_map<string, Scene*> allScenes;

};

