#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera3D.h"

class Camera3DController {
public:
	Camera3DController();
	~Camera3DController();

	void Inject(Camera3D* camera, GLFWwindow* window);
	void Update(const float& dt);
	void InitCursorPos();

public:
	Camera3D* camera;
	GLFWwindow* window;
	float moveSpeed = 0.1f;
	float rotateSpeed = 0.1f;

private:
	double m_cursorPosX;
	double m_cursorPosY;

};
