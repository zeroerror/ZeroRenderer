#pragma once
#include "DirectLight.h"
#include "Cube.h"
#include "Model.h"
#include "Rectangle.h"
#include "Camera3D.h"
#include "GameObject.h"

class Scene {

public:
	Scene();
	~Scene();

	// todo remove
	Camera3D* camera;
	DirectLight* directLight;
	Rectangle* depthMapImage;
	Cube* lightCube;
	std::vector<Cube*>* cubes;
	std::vector<Model*>* models;

	vector<GameObject*> gameObjects;

	void SerializeTo(const string& path);
	void DeserializeFrom(const string& path);


};

