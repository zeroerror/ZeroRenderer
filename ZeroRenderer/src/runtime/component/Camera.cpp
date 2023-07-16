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

Camera::Camera() {
	nearPlane = 0.1f;
	farPlane = 100.0f;
	fov = 45.0f;
	orthoSize = 20.0f;
	cameraType = CameraType::Perspective;

	scrWidth = 1920;
	scrHeight = 1080;
	componentType = ComponentType_Camera;
}

Camera::~Camera() {
	std::cout << "Camera::~Camera()" << std::endl;
}

static float u_Time = 0.0f;
void Camera::Update(const float& dt) {
	u_Time += dt;
}

glm::mat4 Camera::GetMVPMatrix_Ortho(const glm::vec3& pos) const {
	glm::vec3 cameraPos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), pos - cameraPos);

	glm::quat cameraRot = transform->GetRotation();
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

	glm::mat4 proj = glm::orthoRH(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 Camera::GetMVPMatrix_Perspective(const glm::vec3& pos) const {
	glm::vec3 cameraForward = transform->GetForward();
	glm::vec3 cameraPos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), pos - cameraPos);

	glm::quat cameraRot = transform->GetRotation();
	cameraRot = cameraRot * glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

	float fovRadians = glm::radians(fov);
	float aspectRatio = scrWidth / scrHeight;
	glm::mat4 proj = glm::perspectiveRH(fovRadians, aspectRatio, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}
