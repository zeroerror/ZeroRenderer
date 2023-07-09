#include "Camera3D.h"
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

Camera3D::Camera3D() {
	nearPlane = 0.1f;
	farPlane = 100.0f;
	fov = 45.0f;
	orthoSize = 20.0f;
	cameraType = CameraType::Perspective;

	scrWidth = 1920;
	scrHeight = 1080;
	componentType = ComponentType_Camera3D;
}

Camera3D::~Camera3D() {
	std::cout << "Camera3D::~Camera3D()" << std::endl;
}

static float u_Time = 0.0f;
void Camera3D::Update(const float& dt) {
	u_Time += dt;
}

glm::mat4 Camera3D::GetMVPMatrix_Ortho(const glm::vec3& pos) const {
	glm::vec3 cameraPos = transform->GetPosition();
	glm::mat4 model = glm::translate(glm::mat4(1), pos - cameraPos);

	glm::quat cameraRot = transform->GetRotation();
	glm::mat4 view = glm::toMat4(glm::inverse(cameraRot));

	glm::mat4 proj = glm::orthoRH(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);

	glm::mat4 mvp = proj * view * model;

	return mvp;
}

glm::mat4 Camera3D::GetMVPMatrix_Perspective(const glm::vec3& pos) const {
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


void Camera3D::SerializeTo(std::stringstream& ss) {
	ss << "componentType: " << componentType << std::endl;
	ss << "scrWidth: " << scrWidth << std::endl;
	ss << "scrHeight: " << scrHeight << std::endl;
	ss << "fov: " << fov << std::endl;
	ss << "nearPlane: " << nearPlane << std::endl;
	ss << "farPlane: " << farPlane << std::endl;
	ss << "orthoSize: " << orthoSize << std::endl;
	ss << "componentEnd" << std::endl;
}

void Camera3D::DeserializeFrom(std::stringstream& ss) {
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "componentEnd") {
			break;
		}
		if (key == "scrWidth:") {
			iss >> key;
			scrWidth = atof(key.c_str());
		}
		else if (key == "scrHeight:") {
			iss >> key;
			scrHeight = atof(key.c_str());
		}
		else if (key == "fov:") {
			iss >> key;
			fov = atof(key.c_str());
		}
		else if (key == "nearPlane:") {
			iss >> key;
			nearPlane = atof(key.c_str());
		}
		else if (key == "farPlane:") {
			iss >> key;
			farPlane = atof(key.c_str());
		}
		else if (key == "orthoSize:") {
			iss >> key;
			orthoSize = atof(key.c_str());
		}
	}
}
