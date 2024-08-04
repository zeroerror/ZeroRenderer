#include "DirectLight.h"
#include <glm/gtx/quaternion.hpp>

DirectLight::DirectLight()
{
	componentType = ComponentType_DirectLight;
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	orthoSize = 20.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;
}

DirectLight::~DirectLight()
{
}
