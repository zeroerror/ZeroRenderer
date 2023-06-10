#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Test.h"
#include "Camera3D.h"
#include "Camera3DController.h"
#include "Cube.h"

namespace test {

	class LightTest :public Test {
	public:
		LightTest();
		~LightTest();

		void OnRender() override;
		void OnUpdate(const float& deltaTime) override;

		void Ctor(GLFWwindow* window, const int& screen_width, const int& screen_height);
	public:
		Camera3D camera;
		GLFWwindow* window;
		const float moveSpeed = 0.1f;
		const float rotateSpeed = 0.1f;

	private:
		int m_screen_width;
		int m_screen_height;
		Cube* m_cubes[10];
		Camera3DController m_cameraController;
	};

}