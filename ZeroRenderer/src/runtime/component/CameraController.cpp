#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include "CameraController.h"

CameraController::CameraController() {
}
CameraController::~CameraController() {
	std::cout << "CameraController::~CameraController()" << std::endl;
}

void CameraController::Inject(Camera* camera, GLFWwindow* window) {
	this->camera = camera;
	this->window = window;
	glfwGetCursorPos(window, &m_cursorPosX, &m_cursorPosY);
}

void CameraController::Update(const float& dt) {
	Transform* camTrans = camera->transform;

	//- Camera Move
	if (glfwGetKey(window, GLFW_KEY_A)) {
		glm::vec3 right = camTrans->GetRight();
		camTrans->SetPosition(camTrans->GetPosition() + right * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		glm::vec3 right = camTrans->GetRight();
		camTrans->SetPosition(camTrans->GetPosition() - right * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		glm::vec3 forward = camTrans->GetForward();
		camTrans->SetPosition(camTrans->GetPosition() + forward * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		glm::vec3 forward = camTrans->GetForward();
		camTrans->SetPosition(camTrans->GetPosition() - forward * moveSpeed);
	}

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	double cursorOffsetX = xPos - m_cursorPosX;
	double cursorOffsetY = yPos - m_cursorPosY;
	m_cursorPosY = yPos;
	m_cursorPosX = xPos;
	float yawRadius = -glm::radians(cursorOffsetX * rotateSpeed);
	float pitchRadius = glm::radians(cursorOffsetY * rotateSpeed);
	glm::quat camRot = camTrans->GetRotation();
	camRot =  glm::quat(glm::vec3(0, yawRadius, 0)) * camRot * glm::quat(glm::vec3(pitchRadius, 0, 0)) ;
	camTrans->SetRotation(camRot);

	camera->Update(dt);

	glm::vec3 euler = glm::eulerAngles(camRot);
	// std::cout << "Camera Rotation " << euler.x * 57.2958f << " " << euler.y * 57.2958f << " " << euler.z * 57.2958f << std::endl;
	// std::cout << "Camera Position " << glm::to_string(camera->transform->GetPosition()) << std::endl;
}

void CameraController::InitCursorPos() {
	glfwGetCursorPos(window, &m_cursorPosX, &m_cursorPosY);
}
