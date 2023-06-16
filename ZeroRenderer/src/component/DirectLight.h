#pragma once

#include "Transform.h"
#include "ShadowType.h"

class DirectLight {
public:
	DirectLight();
	~DirectLight();

public:
	Transform* transform;
	glm::vec3 color;
	ShadowType shadowType;
	float nearPlane;
	float farPlane;
	float orthoSize;
	
	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3& pos) const;
	glm::vec3 GetLightDirection() const;
};
