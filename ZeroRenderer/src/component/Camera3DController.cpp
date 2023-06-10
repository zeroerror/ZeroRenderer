#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include "Camera3DController.h"

Camera3DController::Camera3DController() {
}
Camera3DController::~Camera3DController() {}

void Camera3DController::Inject(Camera3D* camera, GLFWwindow* window) {
	this->camera = camera;
	this->window = window;
	glfwGetCursorPos(window, &m_cursorPosX, &m_cursorPosY);
}

void Camera3DController::Update(const float& dt) {
	Transform& camTrans = camera->transform;

	//- Camera Move
	if (glfwGetKey(window, GLFW_KEY_A)) {
		glm::vec3 right = camTrans.GetRight();
		camTrans.SetPosition(camTrans.GetPosition() + -right * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		glm::vec3 right = camTrans.GetRight();
		camTrans.SetPosition(camTrans.GetPosition() + right * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		glm::vec3 forward = camTrans.GetForward();
		camTrans.SetPosition(camTrans.GetPosition() + -forward * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		glm::vec3 forward = camTrans.GetForward();
		camTrans.SetPosition(camTrans.GetPosition() + forward * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	double cursorOffsetX = xPos - m_cursorPosX;
	double cursorOffsetY = yPos - m_cursorPosY;
	m_cursorPosY = yPos;
	m_cursorPosX = xPos;
	float yawRadius = -glm::radians(cursorOffsetX * rotateSpeed);
	float pitchRadius = -glm::radians(cursorOffsetY * rotateSpeed);
	glm::quat camRot = camTrans.GetRotation();
	camRot = glm::quat(glm::vec3(0, yawRadius, 0)) * camRot * glm::quat(glm::vec3(pitchRadius, 0, 0));
	camTrans.SetRotation(camRot);

	camera->Update(dt);

	glm::vec3 euler = glm::eulerAngles(camRot);
	//std::cout << "Camera3D Rotation " << euler.x * 57.2958f << " " << euler.y * 57.2958f << " " << euler.z * 57.2958f << std::endl;
	//std::cout << "Camera3D Position " << glm::to_string(camera->transform.GetPosition()) << std::endl;
}

void Camera3DController::SetMoveSpeed(const float& moveSpeed) {
	this->moveSpeed = moveSpeed;
}

void Camera3DController::SetRotateSpeed(const float& rotateSpeed) {
	this->rotateSpeed = rotateSpeed;
}