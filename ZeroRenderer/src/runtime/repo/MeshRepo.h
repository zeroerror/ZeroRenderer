#pragma once
#include <string>
#include <Mesh.h>
#include <unordered_map>

using namespace std;

class MeshRepo {

public:
	MeshRepo();
	~MeshRepo();

	bool TryAddMesh(const string& modelGUID, const int& index, Mesh*& mesh);
	bool TryGetMesh(const string& modelGUID, const int& index, Mesh*& mesh);

	Mesh* DefaultCubeMesh();

private:
	unordered_map<string, Mesh*> allMeshs;
	Mesh* _defaultCubeMesh;
	inline void _CreateDefaultCubeMesh();

};

