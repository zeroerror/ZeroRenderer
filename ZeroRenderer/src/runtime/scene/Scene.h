#pragma once
#include "DirectLight.h"
#include "Cube.h"
#include "Camera.h"
#include "GameObject.h"
using namespace std;

class Scene {

public:
	Scene();
	~Scene();

	vector<GameObject*> gameObjects;

	GameObject* Find(const string& name);
	GameObject* Find(const int& gid);

};

