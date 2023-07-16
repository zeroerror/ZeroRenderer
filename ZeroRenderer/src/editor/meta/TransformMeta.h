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

};

