#pragma once

#include "Transform.h"
#include "ShadowType.h"
#include "ComponentMeta.h"

class DirectLightMeta :public ComponentMeta {
public:
	DirectLightMeta() { componentType = ComponentType_DirectLight; }

	glm::vec3 color;
	ShadowType shadowType;
	float scrWidth;
	float scrHeight;
	float fov;
	float nearPlane;
	float farPlane;
	float orthoSize;

};

