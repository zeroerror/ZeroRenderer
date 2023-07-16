#pragma once

#include "Transform.h"
#include "ShadowType.h"

class SpotLight {
public:
	SpotLight() {
		transform = new Transform();
	};
	~SpotLight() {};


public:
    Transform* transform;
	glm::vec3 color;
	ShadowType shadowType;

};
