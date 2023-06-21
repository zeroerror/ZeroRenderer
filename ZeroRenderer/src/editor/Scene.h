#pragma once
#include "DirectLight.h"
#include "Cube.h"
#include "Model.h"
#include "Rectangle.h"
#include "Camera3D.h"

class Scene {

public:
	Scene();
	~Scene();

	Camera3D* camera;
	DirectLight* directLight;
	Rectangle* depthMapImage;
	Cube* lightCube;
	std::vector<Cube*>* cubes;
	std::vector<Model*>* models;

};

