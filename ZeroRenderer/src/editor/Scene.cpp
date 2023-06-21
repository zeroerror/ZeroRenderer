#include "Scene.h"

Scene::Scene() {
    camera = new Camera3D();
    directLight = new DirectLight();
    depthMapImage = new Rectangle();
    lightCube = new Cube();
    cubes = new std::vector<Cube*>();
    models = new std::vector<Model*>();
}

Scene::~Scene() {
    delete camera;
    delete directLight;
    delete lightCube;
    delete depthMapImage;
    for (auto cube : *cubes) {
		delete cube;
	}
    for (auto model : *models) {
		delete model;
	}
}