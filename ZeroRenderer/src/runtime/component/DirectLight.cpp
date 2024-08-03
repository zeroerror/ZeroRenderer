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

glm::mat4 DirectLight::GetMVPMatrix_Ortho(const glm::vec3 &pos) const
{
	glm::vec3 lightForward = transform->GetForward();
	glm::vec3 lightPos = transform->GetPosition() - lightForward * 20.0f;
	glm::mat4 model = glm::translate(glm::mat4(1), pos - lightPos);

	glm::quat lightRot = transform->GetRotation();
	lightRot = lightRot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(lightRot));

	glm::mat4 proj = glm::orthoRH(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 DirectLight::GetMVPMatrix_Perspective(const glm::vec3 &modelPos) const
{
	// 计算光源位置
	glm::vec3 forwad = transform->GetForward();
	glm::vec3 pos = transform->GetPosition() - forwad * 10.0f;

	// 模型矩阵：将物体从光源视角的位置调整
	glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPos - pos);

	// 视图矩阵：光源的旋转矩阵
	glm::quat lightRot = transform->GetRotation();
	lightRot = lightRot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(lightRot));

	// 透视投影矩阵
	float fovRadians = glm::radians(fov);
	float aspectRatio = scrWidth / scrHeight;
	glm::mat4 proj = glm::perspectiveRH(fovRadians, aspectRatio, nearPlane, farPlane);

	// 计算 MVP 矩阵
	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::vec3 DirectLight::GetLightDirection() const
{
	glm::vec3 forward = transform->GetForward();
	glm::vec3 lightDir = -forward;
	return lightDir;
}
