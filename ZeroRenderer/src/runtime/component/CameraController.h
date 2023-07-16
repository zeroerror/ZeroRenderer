#pragma once
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class CameraController {
public:
	CameraController();
	~CameraController();

	void Inject(Camera* camera, GLFWwindow* window);
	void Update(const float& dt);
	void InitCursorPos();

public:
	Camera* camera;
	GLFWwindow* window;
	float moveSpeed = 0.1f;
	float rotateSpeed = 0.1f;

private:
	double m_cursorPosX;
	double m_cursorPosY;

};
