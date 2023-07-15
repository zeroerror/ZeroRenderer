#pragma once 
#include <string>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;
class EditorModelManager {

public:
	static bool TryLoadModel(const string& path, const aiScene*& aScene);

private:
	static Assimp::Importer _importer;
	static unordered_map<string, const aiScene*> _modelMap;


};