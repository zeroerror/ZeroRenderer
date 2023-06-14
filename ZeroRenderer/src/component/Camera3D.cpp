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
}

Camera3D::~Camera3D() {
	std::cout << "Camera3D::~Camera3D()" << std::endl;
}

static float u_Time = 0.0f;
void Camera3D::Update(const float& dt) {
	u_Time += dt;
}

void Camera3D::Render(const glm::vec3 modPosition, const glm::quat modRotation, const VertexArray* va, const IndexBuffer* ib, const Shader* shader, const Texture* texture) {

}

glm::mat4 Camera3D::GetMVPMatrix_Ortho(const glm::vec3& pos, const glm::quat& rot) {
	glm::vec3 cameraPos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), pos - cameraPos);

	glm::quat cameraRot = transform->GetRotation();
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

	float halfWidth = 10;
	float halfHeight = 10;
	glm::mat4 proj = glm::orthoRH(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 Camera3D::GetMVPMatrix_Perspective(const glm::vec3& pos) {
	glm::vec3 cameraPos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), pos - cameraPos);

	glm::quat cameraRot = transform->GetRotation();
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

	float constexpr fov = glm::radians(45.0f); 
	float aspectRatio = width / height;
	glm::mat4 proj = glm::perspectiveRH(fov, aspectRatio, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}


