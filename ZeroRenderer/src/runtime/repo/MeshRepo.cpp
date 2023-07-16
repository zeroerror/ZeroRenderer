#include "MeshRepo.h"
#include "EditorDatabase.h"
#include <iostream>

MeshRepo::MeshRepo() {
	allMeshs = unordered_map<string, Mesh*>();
}

MeshRepo::~MeshRepo() {
	cout << "MeshRepo::~MeshRepo() " << endl;
	for (auto& mesh : allMeshs) {
		mesh.second->~Mesh();
	}
}

bool MeshRepo::TryAddMesh(const string& modelGUID, const int& index, Mesh*& mesh) {
	if (!EditorDatabase::GUIDExist(modelGUID)) {
		return false;
	}

	if (TryGetMesh(modelGUID, index, mesh)) {
		return false;
	}

	stringstream key;
	key << modelGUID << index;
	allMeshs.insert(pair<string, Mesh*>(key.str(), mesh));

	cout << "MeshRepo::TryAddMesh() " << key.str() << endl;

	return true;
}

bool MeshRepo::TryGetMesh(const string& modelGUID, const int& index, Mesh*& mesh) {
	stringstream key;
	key << modelGUID << index;
	unordered_map<string, Mesh*>::iterator it = allMeshs.find(key.str());
	if (it == allMeshs.end()) {
		return false;
	}

	mesh = it->second;
	return true;
}