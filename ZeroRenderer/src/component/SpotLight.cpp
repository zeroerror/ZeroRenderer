#include "SpotLight.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 SpotLight::GetLightSpaceMatrix() {
    glm::quat cameraRot = transform->GetRotation();
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

    float orthoSize = 10.0f;
    glm::mat4 projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);

	return projection * view;
}