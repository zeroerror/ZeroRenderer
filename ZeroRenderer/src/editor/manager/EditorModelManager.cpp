#include "EditorModelManager.h"
#include "FileHelper.h"

#include <iostream>
#include <assimp/postprocess.h>

unordered_map<string, const aiScene*> EditorModelManager::_modelMap = unordered_map<string, const aiScene*>();
Assimp::Importer EditorModelManager::_importer;

bool EditorModelManager::TryLoadModel(const string& path, const aiScene*& aScene) {
	string norPath = FileHelper::NormalizedPath(path);
	unordered_map<string, const aiScene*>::iterator it = _modelMap.find(norPath);

	if (it != _modelMap.end()) {
		aScene = _modelMap.at(norPath);
	}
	else {
		aScene = _importer.ReadFile(norPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!aScene || aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aScene->mRootNode) {
			cout << "  ################################# ERROR::ASSIMP::" << _importer.GetErrorString() << endl;
			return false;
		}

		_modelMap.insert(pair<string, const aiScene*>(norPath, aScene));
	}

	return true;
}

