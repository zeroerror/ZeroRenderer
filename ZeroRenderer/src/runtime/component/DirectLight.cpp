#include "DirectLight.h"
#include <glm/gtx/quaternion.hpp>

DirectLight::DirectLight()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	componentType = ComponentType_DirectLight;
}

DirectLight::~DirectLight()
{
}
