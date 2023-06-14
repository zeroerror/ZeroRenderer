#include "PipelineTest.h"

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLDebug.h>
#include <glm/gtx/quaternion.hpp>

#include "Camera3DController.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "SpotLight.h"

namespace test {

	PipelineTest::PipelineTest() {
		m_assetPath2AssetID = std::unordered_map<std::string, unsigned int>();
		m_assetID2AssetPath = std::unordered_map<unsigned int, std::string>();

		m_assetID2shaderID = std::unordered_map<unsigned int, unsigned int>();
		m_assetID2textureID = std::unordered_map<unsigned int, unsigned int>();

		m_assetPath2shaderID = std::unordered_map<std::string, unsigned int>();
		m_assetPath2textureID = std::unordered_map<std::string, unsigned int>();

		m_shaderRepo = new ShaderRepo();
		m_textureRepo = new TextureRepo();

		m_cameraControllerEnabled = false;
	}

	PipelineTest::~PipelineTest() {
		std::cout << "PipelineTest::~PipelineTest()" << std::endl;
		GLCall(glfwMakeContextCurrent(window));
		for (int i = 0; i < 10; i++) {
			delete m_cubes[i];
		}
		delete m_shaderRepo;
		delete m_textureRepo;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwDestroyWindow(window);
	}

	void PipelineTest::Init() {
		m_screen_width = 1920;
		m_screen_height = 1080;

		LoadAssetsDatabase();
		InitOpenGL();
		InitImGui();
		LoadCamera();
		LoadShaders();
		LoadTextures();

		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			PipelineTest* camera3DCubeTest = static_cast<PipelineTest*>(glfwGetWindowUserPointer(window));
			Transform& camTrans = camera3DCubeTest->camera.transform;
			glm::vec3 pos = camTrans.GetPosition();
			glm::vec3 forward = camTrans.GetForward();
			pos += forward * static_cast<float>(yoffset * camera3DCubeTest->moveSpeed);
			camTrans.SetPosition(pos);
		});

		// ====== Scene
		Material* defaultMaterial = new Material();
		defaultMaterial->SetDiffuseTexture(1000);
		defaultMaterial->SetShader(1000);

		Material* lightMaterial = new Material();
		lightMaterial->SetDiffuseTexture(1000);
		lightMaterial->SetShader(2000);

		Cube* lightCube = Cube::CreateCube(0.2f, 0.2f, 0.2f);
		lightCube->transform.SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
		lightCube->transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		lightCube->material = defaultMaterial;
		m_cubes[0] = lightCube;

		Cube* cube = Cube::CreateCube(5.0f, 5.0f, 5.0f);
		cube->transform.SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
		cube->transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		cube->material = lightMaterial;
		m_cubes[1] = cube;

		for (int i = 2; i < 10; i++) {
			Cube* cube = Cube::CreateCube(i * 2.0f, 2.0f, 2.0);
			cube->transform.SetPosition(glm::vec3(i * 2.0f, i * 2.0f, i * 2.0f));
			cube->transform.SetRotation(glm::quat(glm::vec3(0.0f, glm::radians(i * 18.0f), 0.0f)));
			cube->material = lightMaterial;
			m_cubes[i] = cube;
		}

		// Light 
		m_spotLight = SpotLight();
		m_spotLight.transform.SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));
		m_spotLight.color = glm::vec4(0.8f, 0.8f, 0.8f, 0.8f);
	}

	void PipelineTest::OnUpdate(const float& deltaTime) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
			m_cameraControllerEnabled = true;
			m_cameraController.InitCursorPos();
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			m_cameraControllerEnabled = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (m_cameraControllerEnabled) {
			m_cameraController.Update(deltaTime);
		}
		camera.Update(deltaTime);

		// - Light Position Control - For Debug
		glm::vec3 forward = -camera.transform.GetForward();
		forward.y = 0;
		forward = glm::normalize(forward);
		glm::vec3 right = glm::cross(glm::vec3(0, -1, 0), forward);

		if (!m_cameraControllerEnabled) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight.transform.GetPosition();
				pos += forward * deltaTime * 10.0f;
				m_spotLight.transform.SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight.transform.GetPosition();
				pos -= forward * deltaTime * 10.0f;
				m_spotLight.transform.SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight.transform.GetPosition();
				pos -= right * deltaTime * 10.0f;
				m_spotLight.transform.SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight.transform.GetPosition();
				pos += right * deltaTime * 10.0f;
				m_spotLight.transform.SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight.transform.GetPosition();
				pos.y += deltaTime * 10.0f;
				m_spotLight.transform.SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight.transform.GetPosition();
				pos.y -= deltaTime * 10.0f;
				m_spotLight.transform.SetPosition(pos);
			}
		}
	}

	void PipelineTest::OnRender() {
		Repaint();
		RenderObject();
		if (glfwWindowShouldClose(window)) {
			isClosed = true;
		}
	}

	void PipelineTest::OnImGuiRender() {
		ImGui::SetCurrentContext(imguiContext);
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Light Menu");
		glm::vec3 lightColor = m_spotLight.color;
		ImGui::SliderFloat3("Spot Light Color", &lightColor.x, 0.0f, 1.0f);
		m_spotLight.color = lightColor;
		ImGui::End();

		ImGui::Begin("Camera Controller Menu");
		ImGui::SliderFloat("Camera Move Speed", &m_cameraController.moveSpeed, 0.0f, 1.0f);
		ImGui::SliderFloat("Camera Rotate Speed", &m_cameraController.rotateSpeed, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// - cube 0 show light pos. - For Debug
		m_cubes[0]->transform.SetPosition(m_spotLight.transform.GetPosition());

		CallGL();
	}

	void PipelineTest::LoadAssetsDatabase() {
		m_assetPath2AssetID.insert(std::pair<std::string, unsigned int>("Res/Textures/jerry.png", 1000));
		m_assetPath2AssetID.insert(std::pair<std::string, unsigned int>("Res/Textures/room.png", 2000));

		m_assetID2AssetPath.insert(std::pair<unsigned int, std::string>(1000, "Res/Textures/jerry.png"));
		m_assetID2AssetPath.insert(std::pair<unsigned int, std::string>(2000, "Res/Textures/room.png"));
	}

	void PipelineTest::InitOpenGL() {
		/* Initialize the library */
		if (!glfwInit())
			return;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		int screen_width = 1280;
		int screen_height = 1080;
		window = glfwCreateWindow(screen_width, screen_height, "Pipeline Test Window", NULL, NULL);
		if (!window) {
			GLCall(glfwTerminate());
			return;
		}

		// GLFW Context
		GLCall(glfwMakeContextCurrent(window));

		// Enable VSync
		GLCall(glfwSwapInterval(1));

		if (glewInit() != GLEW_OK)
			std::cout << "ERROR" << std::endl;

		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void PipelineTest::InitImGui() {
		IMGUI_CHECKVERSION();
		imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(imguiContext);

		ImGui::StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void PipelineTest::LoadCamera() {
		camera = Camera3D();
		camera.width = m_screen_width;
		camera.height = m_screen_height;
		camera.depth = 1000;
		camera.transform.SetPosition(glm::vec3(0, 0, 20));
		camera.transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		m_cameraController = Camera3DController();
		m_cameraController.Inject(&camera, window);
	}

	void PipelineTest::LoadShaders() {
		unsigned int shaderID = m_shaderRepo->LoadShader("Res/Shader/Default.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(1000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("Res/Shader/Default.shader", shaderID));

		shaderID = m_shaderRepo->LoadShader("Res/Shader/DefaultLight.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(2000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("Res/Shader/DefaultLight.shader", shaderID));
	}

	void PipelineTest::LoadTextures() {
		unsigned int textureID = m_textureRepo->LoadTexture("Res/Textures/jerry.png");
		m_assetID2textureID.insert(std::pair<unsigned int, unsigned int>(1000, textureID));
		m_assetPath2textureID.insert(std::pair<std::string, unsigned int>("Res/Textures/jerry.png", textureID));

		textureID = m_textureRepo->LoadTexture("Res/Textures/room.png");
		m_assetID2textureID.insert(std::pair<unsigned int, unsigned int>(2000, textureID));
		m_assetPath2textureID.insert(std::pair<std::string, unsigned int>("Res/Textures/room.png", textureID));
	}

	void PipelineTest::Repaint() {
		GLCall(glfwMakeContextCurrent(window));
		GLCall(glClearColor(0.8f, 1.0f, 1.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glDepthMask(GL_TRUE));
	}

	void PipelineTest::RenderObject() {
		glm::vec3 lightPos = m_spotLight.transform.GetPosition();
		for (size_t i = 0; i < 10; i++) {
			Cube* cube = m_cubes[i];
			Material* material = cube->material;

			unsigned textureID = m_assetID2textureID[material->diffuseTextureAssetID];
			Texture* texture = m_textureRepo->GetTexture(textureID);
			texture->Bind();

			unsigned shaderID = m_assetID2shaderID[material->shaderAssetID];
			Shader* shader = m_shaderRepo->GetShader(shaderID);
			shader->Bind();

			shader->SetUniform1i("u_texture", 0);
			shader->SetUniformMat4f("u_mvp", camera.GetMVPMatrix_Perspective(cube->transform.GetPosition()));
			shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(cube->transform.GetRotation()));

			glm::vec3 modPosition = cube->transform.GetPosition();
			shader->SetUniform3f("u_modPosition", modPosition.x, modPosition.y, modPosition.z);

			shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
			shader->SetUniform3f("u_lightColor", m_spotLight.color.x, m_spotLight.color.y, m_spotLight.color.z);

			VertexArray* va = cube->va;
			IndexBuffer* ib = cube->ib;
			va->Bind();
			ib->Bind();

			GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}

	void PipelineTest::CallGL() {
		GLCall(glfwMakeContextCurrent(window));
		GLCall(glfwSwapBuffers(window));
	}
}