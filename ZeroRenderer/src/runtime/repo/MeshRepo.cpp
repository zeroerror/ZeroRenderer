#include "MeshRepo.h"
#include "EditorDatabase.h"
#include <iostream>

MeshRepo::MeshRepo()
{
	allMeshs = unordered_map<string, Mesh *>();
	_CreateDefaultCubeMesh();
}

MeshRepo::~MeshRepo()
{
	cout << "MeshRepo::~MeshRepo() " << endl;
	for (auto &mesh : allMeshs)
	{
		mesh.second->~Mesh();
	}
}

void MeshRepo::_CreateDefaultCubeMesh()
{
	Mesh *mesh = new Mesh();
	mesh->meshName = "cube";
	vector<Vertex *> *vertices = new vector<Vertex *>();
	vector<unsigned int> *indices = new vector<unsigned int>();

	// 背面
	vec2 tex1 = vec2(0.0f, 0.0f);
	vec3 p1 = vec3(0.5f, -0.5f, -0.5f);
	vec3 nor1 = glm::normalize(p1);
	Vertex *v1 = new Vertex(p1, nor1, tex1);
	vertices->push_back(v1);

	vec2 tex2 = vec2(1.0f, 0.0f);
	vec3 p2 = vec3(-0.5f, -0.5f, -0.5f);
	vec3 nor2 = glm::normalize(p2);
	Vertex *v2 = new Vertex(p2, nor2, tex2);
	vertices->push_back(v2);

	vec2 tex3 = vec2(1.0f, 1.0f);
	vec3 p3 = vec3(-0.5f, 0.5f, -0.5f);
	vec3 nor3 = glm::normalize(p3);
	Vertex *v3 = new Vertex(p3, nor3, tex3);
	vertices->push_back(v3);

	vec2 tex4 = vec2(0.0f, 1.0f);
	vec3 p4 = vec3(0.5f, 0.5f, -0.5f);
	vec3 nor4 = glm::normalize(p4);
	Vertex *v4 = new Vertex(p4, nor4, tex4);
	vertices->push_back(v4);

	// 正面
	vec2 tex5 = vec2(1.0f, 0.0f);
	vec3 p5 = vec3(0.5f, -0.5f, 0.5f);
	vec3 nor5 = glm::normalize(p5);
	Vertex *v5 = new Vertex(p5, nor5, tex5);
	vertices->push_back(v5);

	vec2 tex6 = vec2(0.0f, 0.0f);
	vec3 p6 = vec3(-0.5f, -0.5f, 0.5f);
	vec3 nor6 = glm::normalize(p6);
	Vertex *v6 = new Vertex(p6, nor6, tex6);
	vertices->push_back(v6);

	vec2 tex7 = vec2(0.0f, 1.0f);
	vec3 p7 = vec3(-0.5f, 0.5f, 0.5f);
	vec3 nor7 = glm::normalize(p7);
	Vertex *v7 = new Vertex(p7, nor7, tex7);
	vertices->push_back(v7);

	vec2 tex8 = vec2(1.0f, 1.0f);
	vec3 p8 = vec3(0.5f, 0.5f, 0.5f);
	vec3 nor8 = glm::normalize(p8);
	Vertex *v8 = new Vertex(p8, nor8, tex8);
	vertices->push_back(v8);

	// 索引
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);

	indices->push_back(4);
	indices->push_back(5);
	indices->push_back(6);
	indices->push_back(6);
	indices->push_back(7);
	indices->push_back(4);

	indices->push_back(1);
	indices->push_back(5);
	indices->push_back(6);
	indices->push_back(6);
	indices->push_back(2);
	indices->push_back(1);

	indices->push_back(4);
	indices->push_back(0);
	indices->push_back(3);
	indices->push_back(3);
	indices->push_back(7);
	indices->push_back(4);

	indices->push_back(3);
	indices->push_back(2);
	indices->push_back(6);
	indices->push_back(6);
	indices->push_back(7);
	indices->push_back(3);

	indices->push_back(4);
	indices->push_back(5);
	indices->push_back(1);
	indices->push_back(1);
	indices->push_back(0);
	indices->push_back(4);

	mesh->vertices = vertices;
	mesh->indices = indices;

	this->_defaultCubeMesh = mesh;
}

bool MeshRepo::TryAddMesh(const string &modelGUID, const int &index, Mesh *&mesh)
{
	if (!EditorDatabase::GUIDExist(modelGUID))
	{
		return false;
	}

	if (TryGetMesh(modelGUID, index, mesh))
	{
		return false;
	}

	stringstream key;
	key << modelGUID << index;
	allMeshs.insert(pair<string, Mesh *>(key.str(), mesh));

	cout << "MeshRepo::TryAddMesh() " << key.str() << endl;

	return true;
}

bool MeshRepo::TryGetMesh(const string &modelGUID, const int &index, Mesh *&mesh)
{
	if (index == -1001)
	{
		mesh = _defaultCubeMesh;
		return true;
	}

	stringstream key;
	key << modelGUID << index;
	unordered_map<string, Mesh *>::iterator it = allMeshs.find(key.str());
	if (it == allMeshs.end())
	{
		return false;
	}

	mesh = it->second;
	return true;
}

Mesh *MeshRepo::DefaultCubeMesh() { return this->_defaultCubeMesh; }