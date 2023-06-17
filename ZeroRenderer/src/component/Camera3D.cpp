#include "Camera3D.h"
#include "Renderer.h"
#include "GLDebug.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

Camera3D::Camera3D() {
	transform = new Transform();
	nearPlane = 0.1f;
	farPlane = 1000.0f;
	orthoSize = 10.0f;
	fov = 45.0f;
	cameraType = CameraType::Perspective;
}

Camera3D::~Camera3D() {
	std::cout << "Camera3D::~Camera3D()" << std::endl;
}

static float u_Time = 0.0f;
void Camera3D::Update(const float& dt) {
	u_Time += dt;
}

glm::mat4 Camera3D::GetMVPMatrix_Ortho(const glm::vec3& pos) const{
	glm::vec3 cameraPos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), pos - cameraPos);

	glm::quat cameraRot = transform->GetRotation();
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

	glm::mat4 proj = glm::orthoRH(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 Camera3D::GetMVPMatrix_Perspective(const glm::vec3& pos) const{
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


