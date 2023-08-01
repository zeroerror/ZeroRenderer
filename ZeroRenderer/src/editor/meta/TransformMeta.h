#pragma once
#include "CameraType.h"
#include "ComponentMeta.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class TransformMeta :public ComponentMeta
{

public:
	TransformMeta();
	~TransformMeta();

	vec3 position;
	quat rotation;
	vec3 scale;

public:
	int gid;
	int fatherGID;
	TransformMeta* father;
	vector<int>* childrenGIDs;
	vector<TransformMeta*>* children;

	void SetFather(TransformMeta* father);
	void AddChild(TransformMeta* child);
	void RemoveChild(const TransformMeta& child);

};

