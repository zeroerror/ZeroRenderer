#pragma once

#include "Transform.h"

class DirectLight {
public:
	DirectLight();
	~DirectLight();
    
public:
	Transform transform;
    glm::vec4 color;
    float intensity;
    
};
