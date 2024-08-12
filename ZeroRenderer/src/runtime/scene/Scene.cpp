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

/**
 * @brief 添加 GameObject 到 Scene
 */
void Scene::AddGameObject(GameObject *go)
{
	if (!go->GetGID()) {
		go->SetGID(allGameObjects->size() + 1);
	}
		
	allGameObjects->push_back(go);
	Transform* trans = go->transform();
	const int fatherGID_forSerialize = trans->fatherGID_forSerialize;
	if (!fatherGID_forSerialize)
	{
		rootGameObjects->push_back(go);
	}
	else if(!trans->GetFather())
	{
		for (int i = 0; i < allGameObjects->size(); i++)
		{
			GameObject *fatherGO = allGameObjects->at(i);
			if (fatherGO->GetGID() == fatherGID_forSerialize)
			{
				trans->SetFather(fatherGO->transform());
				break;
			}
		}
	}
	vector<int> childrenGIDs_forSerialize = trans->childrenGIDs_forSerialize;
	for (int childGID : childrenGIDs_forSerialize)
	{
		for (int i = 0; i < allGameObjects->size(); i++)
		{
			GameObject *childGO = allGameObjects->at(i);
			if (childGO->GetGID() == childGID)
			{
				trans->AddChild(childGO->transform());
				break;
			}
		}
	}
}
