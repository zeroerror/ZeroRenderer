#pragma once

#include "Transform.h"
#include "ShadowType.h"
#include "Component.h"

class DirectLight : public Component
{
public:
	DirectLight();
	~DirectLight();

public:
	glm::vec3 color;
	ShadowType shadowType;
	float orthoSize;
	float nearPlane;
	float farPlane;
};
