#include "Scene.h"
#include "FileHelper.h"
#include "Serialization.h"
using namespace Serialization;

Scene::Scene() {
	//camera = new Camera();
	//directLight = new DirectLight();
	//depthMapImage = new Rectangle();
	//lightCube = new Cube();
	//cubes = new vector<Cube*>();
	//models = new vector<Model*>();
}

Scene::~Scene() {
	//delete camera;
	//delete directLight;
	//delete lightCube;
	//delete depthMapImage;
	//for (auto cube : *cubes) {
	//	delete cube;
	//}
	//for (auto model : *models) {
	//	delete model;
	//}
}

GameObject* Scene::Find(const string& name) {
	for (auto go : gameObjects) {
		if (go->name == name) {
			return go;
		}
	}

	return nullptr;
}