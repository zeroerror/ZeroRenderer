#include "Camera.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "GLDebug.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

Camera::Camera()
{
	componentType = ComponentType_Camera;
	nearPlane = 0.1f;
	farPlane = 100.0f;
	fov = 45.0f;
	orthoSize = 20.0f;
	cameraType = CameraType::Perspective;
	scrWidth = 1920;
	scrHeight = 1080;
}

Camera::~Camera() {}

static float u_Time = 0.0f;
void Camera::Update(const float &dt)
{
	u_Time += dt;
}

glm::mat4 Camera::GetMVPMatrix_Ortho(const glm::vec3 &modelPos) const
{
	return Camera::GetMVPMatrix_Ortho(modelPos, *transform, orthoSize, nearPlane, farPlane);
}

glm::mat4 Camera::GetMVPMatrix_Ortho(const glm::vec3 &modelPos, const Transform &trans, const float &orthoSize, const float &nearPlane, const float &farPlane)
{
	// 模型矩阵
	glm::vec3 pos = trans.GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), modelPos - pos);
	// 视图矩阵
	glm::quat rot = trans.GetRotation();
	rot = rot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(rot));
	// 正交投影矩阵
	glm::mat4 proj = glm::orthoRH(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);
	// MVP 矩阵
	glm::mat4 mvp = proj * view * model;
	return mvp;
}

glm::mat4 Camera::GetMVPMatrix_Perspective(const glm::vec3 &modelPos) const
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
	// MVP 矩阵
	glm::mat4 mvp = proj * view * model;
	return mvp;
}

void Camera::CopyFrom(const Camera &camera)
{
	nearPlane = camera.nearPlane;
	farPlane = camera.farPlane;
	fov = camera.fov;
	orthoSize = camera.orthoSize;
	cameraType = camera.cameraType;
	scrWidth = camera.scrWidth;
	scrHeight = camera.scrHeight;
}
