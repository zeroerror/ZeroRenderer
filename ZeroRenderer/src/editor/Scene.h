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

	// todo remove
	Camera3D* camera;
	DirectLight* directLight;
	Rectangle* depthMapImage;
	Cube* lightCube;
	vector<Cube*>* cubes;
	vector<Model*>* models;

	vector<GameObject*> gameObjects;

};

