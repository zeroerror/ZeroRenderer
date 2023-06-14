#pragma once

#include "Transform.h"
#include "ShadowType.h"

class DirectLight {
public:
	DirectLight() {};
	~DirectLight() {};

public:
	glm::vec3 color;
	ShadowType shadowType;
};
