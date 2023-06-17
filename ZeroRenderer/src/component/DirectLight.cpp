#include "DirectLight.h"
#include <glm/gtx/quaternion.hpp>

DirectLight::DirectLight() {
    transform = new Transform();
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 1000.0f;
	orthoSize = 10.0f;
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    shadowType = ShadowType::Hard;
}

DirectLight::~DirectLight() {
    delete transform;
}

glm::mat4 DirectLight::GetMVPMatrix_Ortho(const glm::vec3& pos) const{
	glm::quat lightRot = transform->GetRotation();
	glm::vec3 lightPos = transform->GetForward() * 100.0f;

	glm::mat4 model = glm::translate(glm::mat4(1), pos - lightPos);

	glm::mat4 view = glm::toMat4(glm::inverse(lightRot));

	glm::mat4 proj = glm::orthoRH(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 DirectLight::GetMVPMatrix_Perspective(const glm::vec3& pos) const{
	glm::vec3 lightForward = transform->GetForward();
	glm::vec3 lightPos = -lightForward * 30.0f;
	glm::mat4 model = glm::translate(glm::mat4(1), pos - lightPos);

	glm::quat lightRot = transform->GetRotation();
	lightRot = lightRot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(lightRot));

	float fovRadians = glm::radians(fov); 
	float aspectRatio = scrWidth / scrHeight;
	glm::mat4 proj = glm::perspectiveRH(fovRadians, aspectRatio, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::vec3 DirectLight::GetLightDirection() const {
	glm::vec3 forward = transform->GetForward();
	glm::vec3 lightDir = -forward;
	return lightDir;
}
