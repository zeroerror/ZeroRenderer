#pragma once
#include "DirectLight.h"
#include "Cube.h"
#include "Model.h"
#include "Rectangle.h"
#include "Camera3D.h"
#include "GameObject.h"
using namespace std;

class Scene {

public:
	Scene();
	~Scene();

	vector<GameObject*> gameObjects;

	GameObject* Find(const string& name);

};

