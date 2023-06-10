#include "Camera3DCubeTest.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

#include "GLDebug.h"
#include "Camera3DController.h"

namespace test {

	Camera3DCubeTest::Camera3DCubeTest() {}
	Camera3DCubeTest::~Camera3DCubeTest() {}

	void Camera3DCubeTest::Ctor(GLFWwindow* window, const int& screen_width, const int& screen_height) {

		// ====== Asset Loading
		shaderManager = ShaderManager();
		unsigned int shader = shaderManager.LoadShader("Res/Shader/Cube.shader");

		textureAssets = TextureManager();
		unsigned int texture = textureAssets.LoadTexture("Res/Textures/jerry.png");

		// ====== Save Asset Reference By AID(Asset ID) to a hash map

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
			Camera3DCubeTest* camera3DCubeTest = static_cast<Camera3DCubeTest*>(glfwGetWindowUserPointer(window));
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

		Cube* cube = Cube::CreateCube(20.0f, 0.5f, 20.0f);
		cube->transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		cube->transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		cube->material = material;
		m_cubes[0] = cube;
		for (int i = 1; i < 10; i++) {
			Cube* cube = Cube::CreateCube(1.0f + i, 1.0f, 1.0f + i);
			cube->transform.SetPosition(glm::vec3(i * 1.0f, i * 1.0f, i * 1.0f));
			cube->transform.SetRotation(glm::angleAxis(glm::radians(18.0f * i), glm::vec3(0.0f, 1.0f, 0.0f)));
			cube->material = material;
			m_cubes[i] = cube;
		}
	}

	void Camera3DCubeTest::OnUpdate(const float& deltaTime) {
		m_cameraController.Update(deltaTime);
		camera.Update(deltaTime);
	}

	void Camera3DCubeTest::OnRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glDepthMask(GL_TRUE));

		for (size_t i = 0; i < 10; i++) {
			Cube* cube = m_cubes[i];

			glm::vec3 modPosition = cube->transform.GetPosition();
			glm::quat modRotation = cube->transform.GetRotation();

			// TODO: This should be set by material
			Material* material = cube->material;
			Shader* shader = material->shader;
			shader->SetUniform1i("u_Texture", 0);
			shader->SetUniformMat4f("u_MVP", camera.GetMVPMatrix_Perspective(modPosition, modRotation));
			shader->SetUniformMat4f("u_ModRotationMatrix", glm::toMat4(modRotation));
			shader->SetUniform4f("u_BlendColor", 0.0f, 0.0f, 0.0f, 1.0f);

			camera.Render(modPosition, modRotation, material, &cube->va, cube->GetIndexBuffer());
		}
	}

}