#include "Scene.h"
#include "FileHelper.h"

Scene::Scene() {
	camera = new Camera3D();
	directLight = new DirectLight();
	depthMapImage = new Rectangle();
	lightCube = new Cube();
	cubes = new vector<Cube*>();
	models = new vector<Model*>();
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

void Scene::SerializeTo(const string& path) {
	stringstream ss;
	ss << "GameObjects: " << endl;
	for (auto go : gameObjects) {
		ss << "GameObject:" << endl;
		go->SerializeTo(ss);
	}
	ss << "---" << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
	delete charResult;
}

void Scene::DeserializeFrom(const string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "GameObjects:") {
			while (getline(ss, line)) {
				iss = istringstream(line);
				if (!(iss >> key)) {
					break;
				}
				if (key == "GameObject:") {
					GameObject* go = new GameObject();
					go->DeserializeFrom(ss);
					gameObjects.push_back(go);
				}
				else if (key == "---") {
					break;
				}
			}
		}
	}
	delete res;
}
