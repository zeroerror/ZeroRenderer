#pragma once

#include "Transform.h"

class SpotLight {
public:
	SpotLight() {};
	~SpotLight() {};

public:
    Transform transform;
	glm::vec3 color;

};
