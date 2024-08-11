#pragma once
#include "DirectLight.h"
#include "Cube.h"
#include "Camera.h"
#include "GameObject.h"
using namespace std;

class Scene
{

public:
	Scene();
	~Scene();

	vector<GameObject *> *rootGameObjects;
	vector<GameObject *> *allGameObjects;

	/**
	 * @brief 根据相对路径查找GameObject
	 * @param relativePath 相对路径, 如: "root/child1/child2"
	 */
	GameObject *FindByPath(const string &relativePath);

	/**
	 * @brief 根据gid全局查找GameObject
	 * @param gid gid
	 */
	GameObject *FindByGid(const int &gid);
};
