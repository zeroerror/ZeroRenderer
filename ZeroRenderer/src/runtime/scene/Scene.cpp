#include "Scene.h"
#include "FileHelper.h"
#include "Serialization.h"
using namespace Serialization;

Scene::Scene()
{
	rootGameObjects = new vector<GameObject *>();
	allGameObjects = new vector<GameObject *>();
}

Scene::~Scene()
{
	for (auto go : *allGameObjects)
	{
		delete go;
	}
	delete allGameObjects;
}

GameObject *Scene::FindByPath(const string &path)
{
	string normalizedPath = FileHelper::NormalizedPath(path);
	auto firstIndex = normalizedPath.find_first_of('/');
	auto name = normalizedPath.substr(0, firstIndex);
	vector<GameObject *> *rootGos = rootGameObjects;
	for (auto go : *rootGos)
	{
		if (go->GetName() == name)
		{
			string realPath = normalizedPath.substr(firstIndex + 1);
			GameObject *result = go->FindByPath(realPath);
			if (result != nullptr)
			{
				return result;
			}
		}
	}
}

GameObject *Scene::FindByGid(const int &gid)
{
	for (auto go : *allGameObjects)
	{
		int id = go->GetGID();
		if (id == gid)
		{
			return go;
		}
	}

	return nullptr;
}
