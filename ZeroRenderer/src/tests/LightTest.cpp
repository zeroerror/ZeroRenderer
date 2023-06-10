#include "LightTest.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <GLDebug.h>
#include <Camera3DController.h>

namespace test {

	LightTest::LightTest() {}
	LightTest::~LightTest() {}

	void LightTest::Ctor(GLFWwindow* window, const int& screen_width, const int& screen_height) {

		// ====== Camera
		camera = Camera3D();
		camera.width = screen_width;
		camera.height = screen_height;
		camera.depth = 1000;
		camera.transform.SetPosition(glm::vec3(0, 0, 20));
		camera.transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));

		this->window = window;

		// 硬件输入事件注册: 设置窗口的用户指针
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			LightTest* camera3DCubeTest = static_cast<LightTest*>(glfwGetWindowUserPointer(window));
			Transform& camTrans = camera3DCubeTest->camera.transform;
			glm::vec3 pos = camTrans.GetPosition();
			glm::vec3 forward = camTrans.GetForward();
			pos += forward * static_cast<float>(yoffset * camera3DCubeTest->moveSpeed);
			camTrans.SetPosition(pos);
			});

		m_screen_width = screen_width;
		m_screen_height = screen_height;

		// ====== Camera Controller
		m_cameraController = Camera3DController();
		m_cameraController.Inject(&camera, window);

		// ====== Cube
		Material* material = new Material();
		material->SetDiffuseTexture(new Texture("Res/Textures/jerry.png"));
		Shader* shader = new Shader("Res/Shader/Cube.shader");
		material->SetShader(shader);

		Cube* cube = Cube::CreateCube(20.0f, 0.5f, 20.0f);
		cube->transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		cube->transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		cube->material = material;
		m_cubes[0] = cube;
		for (int i = 1;i < 10;i++) {
			Cube* cube = Cube::CreateCube(1.0f + i, 1.0f, 1.0f + i);
			cube->transform.SetPosition(glm::vec3(i * 1.0f, i * 1.0f, i * 1.0f));
			cube->transform.SetRotation(glm::angleAxis(glm::radians(18.0f * i), glm::vec3(0.0f, 1.0f, 0.0f)));
			cube->material = material;
			m_cubes[i] = cube;
		}
	}

	void LightTest::OnUpdate(const float& deltaTime) {
		m_cameraController.Update(deltaTime);
		camera.Update(deltaTime);
	}

	void LightTest::OnRender() {
		GLCall(glClearColor(0.8f, 1.0f, 1.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glDepthMask(GL_TRUE));

		for (size_t i = 0; i < 10; i++)
		{
			Cube* cube = m_cubes[i];
			camera.Render(cube->transform.GetPosition(), cube->transform.GetRotation(), cube->material, &cube->va, cube->GetIndexBuffer());
		}
	}

}