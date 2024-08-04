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

glm::mat4 DirectLight::GetMVPMatrix_Ortho(const glm::vec3 &modelPos) const
{
	// 模型矩阵
	glm::vec3 pos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPos - pos);

	// 视图矩阵
	glm::quat rot = transform->GetRotation();
	rot = rot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(rot));

	// 正交投影矩阵
	float halfSize = orthoSize / 2.0f;
	glm::mat4 proj = glm::orthoRH(-halfSize, halfSize, -halfSize, halfSize, nearPlane, farPlane);

	// 计算 MVP 矩阵
	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 DirectLight::GetMVPMatrix_Perspective(const glm::vec3 &modelPos) const
{
	// 模型矩阵
	glm::vec3 pos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPos - pos);

	// 视图矩阵
	glm::quat rot = transform->GetRotation();
	rot = rot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(rot));

	// 透视投影矩阵
	float fovRadians = glm::radians(fov);
	float aspectRatio = scrWidth / scrHeight;
	glm::mat4 proj = glm::perspectiveRH(fovRadians, aspectRatio, nearPlane, farPlane);

	// 计算 MVP 矩阵
	glm::mat4 mvp = proj * view * model;

	return mvp;
}
