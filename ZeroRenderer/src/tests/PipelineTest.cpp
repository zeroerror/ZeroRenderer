#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLDebug.h>
#include <glm/gtx/quaternion.hpp>

#include "Camera3DController.h"
#include "PipelineTest.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"

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
		Material* material = new Material();
		material->SetDiffuseTexture(1000);
		material->SetShader(1000);

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
		
		// Light 
		m_lightPosition = glm::vec3(0.0f, 0.0f, 5.0f); // 光源位置
		m_ambientColor = glm::vec3(0.2f, 0.2f, 0.2f); // 环境光颜色
		m_diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f); // 漫反射颜色
		m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f); // 镜面反射颜色
	}

	void PipelineTest::OnUpdate(const float& deltaTime) {
		if (m_cameraControllerEnabled) {
			m_cameraController.Update(deltaTime);
		}
		camera.Update(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_cameraControllerEnabled = true;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_cameraControllerEnabled = false;
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

		ImGui::Begin("Lighting");
		ImGui::SliderFloat3("Light Position", &m_lightPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Ambient Color", &m_ambientColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse Color", &m_diffuseColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular Color", &m_specularColor.x, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		CallGL();
	}

	void PipelineTest::LoadAssetsDatabase() {
		m_assetPath2AssetID.insert(std::pair<std::string, unsigned int>("Res/Models/Cube.obj", 1000));
		m_assetPath2AssetID.insert(std::pair<std::string, unsigned int>("Res/Textures/jerry.png", 1000));
		m_assetID2AssetPath.insert(std::pair<unsigned int, std::string>(1000, "Res/Models/Cube.obj"));
		m_assetID2AssetPath.insert(std::pair<unsigned int, std::string>(1000, "Res/Textures/jerry.png"));
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

	void PipelineTest::InitImGui(){
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
		unsigned int shaderID = m_shaderRepo->LoadShader("Res/Shader/Cube.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(1000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("Res/Shader/Cube.shader", shaderID));
	}

	void PipelineTest::LoadTextures() {
		unsigned int textureID = m_textureRepo->LoadTexture("Res/Textures/jerry.png");
		m_assetID2textureID.insert(std::pair<unsigned int, unsigned int>(1000, textureID));
		m_assetPath2textureID.insert(std::pair<std::string, unsigned int>("Res/Textures/jerry.png", textureID));
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
		for (size_t i = 0; i < 10; i++) {
			Cube* cube = m_cubes[i];
			Material* material = cube->material;

			unsigned textureID = m_assetID2textureID[material->diffuseTextureAssetID];
			Texture* texture = m_textureRepo->GetTexture(textureID);
			texture->Bind();

			unsigned shaderID = m_assetID2shaderID[material->shaderAssetID];
			Shader* shader = m_shaderRepo->GetShader(shaderID);
			shader->Bind();

			shader->SetUniform1i("u_Texture", 0);
			shader->SetUniformMat4f("u_MVP", camera.GetMVPMatrix_Perspective(cube->transform.GetPosition()));
			shader->SetUniformMat4f("u_ModRotationMatrix", glm::toMat4(cube->transform.GetRotation()));
			shader->SetUniform3f("u_LightPosition", m_lightPosition.x, m_lightPosition.y, m_lightPosition.z);
			shader->SetUniform3f("u_AmbientColor", m_ambientColor.x, m_ambientColor.y, m_ambientColor.z);
			shader->SetUniform3f("u_DiffuseColor", m_diffuseColor.x, m_diffuseColor.y, m_diffuseColor.z);
			shader->SetUniform3f("u_SpecularColor", m_specularColor.x, m_specularColor.y, m_specularColor.z);

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