#pragma once

#include "Transform.h"
#include "ShadowType.h"
#include "Component.h"

class DirectLight :public Component {
public:
	DirectLight();
	~DirectLight();

public:
	glm::vec3 color;
	ShadowType shadowType;
	float scrWidth;
	float scrHeight;
	float fov;
	float nearPlane;
	float farPlane;
	float orthoSize;

	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3& pos) const;
	glm::mat4 GetMVPMatrix_Perspective(const glm::vec3& pos) const;
	glm::vec3 GetLightDirection() const;
};
