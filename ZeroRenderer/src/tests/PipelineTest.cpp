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
#include "ShadowType.h"

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
			Transform* camTrans = camera3DCubeTest->camera->transform;
			glm::vec3 pos = camTrans->GetPosition();
			glm::vec3 forward = camTrans->GetForward();
			pos += forward * static_cast<float>(yoffset * camera3DCubeTest->moveSpeed);
			camTrans->SetPosition(pos);
		});

		// ======================== Scene
		Material* defaultMaterial = new Material();
		defaultMaterial->SetDiffuseTexture(1000);
		defaultMaterial->SetShader(1000);

		Material* defaultLightMaterial = new Material();
		defaultLightMaterial->SetDiffuseTexture(1000);
		defaultLightMaterial->SetShader(2000);

		Material* lightCubeMaterial = new Material();
		lightCubeMaterial->SetDiffuseTexture(2000);
		lightCubeMaterial->SetShader(1000);

		Material* depthMapMaterial = new Material();
		depthMapMaterial->SetDiffuseTexture(1000);
		depthMapMaterial->SetShader(3000);

		// 创建中央光源Cube
		m_lightCube = Cube::CreateCube(0.2f, 0.2f, 0.2f);
		m_lightCube->transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		m_lightCube->transform->SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		m_lightCube->material = lightCubeMaterial;

		// 创建用于展示光照深度贴图的屏幕Cube
		m_depthMapCube = Cube::CreateCube(5.0f, 5.0f, 0.5f);
		m_depthMapCube->transform->SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
		m_depthMapCube->transform->SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		m_depthMapCube->material = depthMapMaterial;

		// 创建环绕中心的Cube
		float radius = 5.0f;
		for (int i = 0; i < 10; i++) {
			float angle = glm::radians((360.0f / 10) * i);
			float x = radius * cos(angle);
			float z = radius * sin(angle);

			Cube* cube = Cube::CreateCube(2.0f, 2.0f, 0.5f);
			cube->transform->SetPosition(glm::vec3(x, 0.0f, z));
			cube->transform->SetRotation(glm::quat(glm::vec3(0.0f, angle, 0.0f)));
			cube->material = defaultLightMaterial;
			m_cubes[i] = cube;
		}

		// Light 
		m_spotLight = new SpotLight();
		m_spotLight->shadowType = ShadowType::Hard;
		m_spotLight->transform->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		m_spotLight->color = glm::vec4(0.8f, 0.8f, 0.8f, 0.8f);
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
		camera->Update(deltaTime);

		// - Light Position Control - For Debug
		glm::vec3 forward = -camera->transform->GetForward();
		forward.y = 0;
		forward = glm::normalize(forward);
		glm::vec3 right = glm::cross(glm::vec3(0, -1, 0), forward);

		if (!m_cameraControllerEnabled) {
			float lightMoveSpeed = 2.0f;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight->transform->GetPosition();
				pos += forward * deltaTime * lightMoveSpeed;
				m_spotLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight->transform->GetPosition();
				pos -= forward * deltaTime * lightMoveSpeed;
				m_spotLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight->transform->GetPosition();
				pos -= right * deltaTime * lightMoveSpeed;
				m_spotLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight->transform->GetPosition();
				pos += right * deltaTime * lightMoveSpeed;
				m_spotLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight->transform->GetPosition();
				pos.y += deltaTime * lightMoveSpeed;
				m_spotLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				glm::vec3 pos = m_spotLight->transform->GetPosition();
				pos.y -= deltaTime * lightMoveSpeed;
				m_spotLight->transform->SetPosition(pos);
			}
			m_lightCube->transform->SetPosition(m_spotLight->transform->GetPosition());
		}
	}

	void PipelineTest::OnRender() {
		Repaint();

		if (m_spotLight->shadowType != ShadowType::None) {
			RenderSceneShadowMap();
			Repaint();
		}

		RenderScene();

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
		glm::vec3 lightColor = m_spotLight->color;
		ImGui::SliderFloat3("Spot Light Color", &lightColor.x, 0.0f, 1.0f);
		m_spotLight->color = lightColor;
		ImGui::End();

		ImGui::Begin("Camera Controller Menu");
		ImGui::SliderFloat("Camera Move Speed", &m_cameraController.moveSpeed, 0.0f, 1.0f);
		ImGui::SliderFloat("Camera Rotate Speed", &m_cameraController.rotateSpeed, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Draw();
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

		// Create framebuffer
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		// Create depth texture
		glGenTextures(1, &m_depthTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

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
		camera = new Camera3D();
		camera->width = m_screen_width;
		camera->height = m_screen_height;
		camera->transform->SetPosition(glm::vec3(0, 0, 20));
		camera->transform->SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		m_cameraController = Camera3DController();
		m_cameraController.Inject(camera, window);
	}

	void PipelineTest::LoadShaders() {
		unsigned int shaderID = m_shaderRepo->LoadShader("Res/Shader/Default.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(1000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("Res/Shader/Default.shader", shaderID));

		shaderID = m_shaderRepo->LoadShader("Res/Shader/DefaultLight.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(2000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("Res/Shader/DefaultLight.shader", shaderID));

		shaderID = m_shaderRepo->LoadShader("Res/Shader/DepthMap.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(3000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("Res/Shader/DepthMap.shader", shaderID));
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

	void PipelineTest::RenderScene() {
		glm::vec3 lightPos = m_spotLight->transform->GetPosition();
		glm::vec3 lightColor = m_spotLight->color;
		for (size_t i = 0; i < 10; i++) {
			Cube* cube = m_cubes[i];
			Material* material = cube->material;
			VertexArray* va = cube->va;
			IndexBuffer* ib = cube->ib;
			glm::vec3 pos = cube->transform->GetPosition();
			glm::quat rot = cube->transform->GetRotation();
			RenderObject(material, va, ib, pos, rot, lightPos, lightColor);
		}

		// - Light Cube
		Material* material = m_lightCube->material;
		VertexArray* va = m_lightCube->va;
		IndexBuffer* ib = m_lightCube->ib;
		glm::vec3 pos = m_lightCube->transform->GetPosition();
		glm::quat rot = m_lightCube->transform->GetRotation();
		RenderObject(material, va, ib, pos, rot, lightPos, lightColor);

		// - Screen Cube 
		material = m_depthMapCube->material;
		va = m_depthMapCube->va;
		ib = m_depthMapCube->ib;
		pos = m_depthMapCube->transform->GetPosition();
		rot = m_depthMapCube->transform->GetRotation();
		unsigned shaderID = m_assetID2shaderID[material->shaderAssetID];
		Shader* shader = m_shaderRepo->GetShader(shaderID);
		shader->Bind();
		shader->SetUniform1i("u_depthMapTexture", 2);
		shader->SetUniform1f("u_nearPlane", camera->nearPlane);
		shader->SetUniform1f("u_farPlane", camera->farPlane);
		RenderObject(material, va, ib, pos, rot, lightPos, lightColor);
	}

	void PipelineTest::RenderSceneShadowMap() {
		Transform* camTrans = camera->transform;
		Transform* lightTrans = m_spotLight->transform;
		glm::vec3 beforeCameraPos = camTrans->GetPosition();
		glm::quat beforeCameraRot = camTrans->GetRotation();
		camTrans->SetPosition(lightTrans->GetPosition());
		camTrans->SetRotation(lightTrans->GetRotation());

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		// Check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "########################## Framebuffer is not complete!" << std::endl;
		}
		else {
			RenderObjectForDepthMap();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		camTrans->SetPosition(beforeCameraPos);
		camTrans->SetRotation(beforeCameraRot);
	}

	void PipelineTest::RenderObjectForDepthMap() {
		glm::vec3 lightPos = m_spotLight->transform->GetPosition();
		glm::vec3 lightColor = m_spotLight->color;
		for (size_t i = 0; i < 10; i++) {
			Cube* cube = m_cubes[i];
			Material* material = cube->material;
			VertexArray* va = cube->va;
			IndexBuffer* ib = cube->ib;
			glm::vec3 pos = cube->transform->GetPosition();
			glm::quat rot = cube->transform->GetRotation();
			RenderObject(material, va, ib, pos, rot, lightPos, lightColor);
		}
	}

	void PipelineTest::RenderObject(Material* material, VertexArray* va, IndexBuffer* ib, const glm::vec3& pos, const glm::quat& rot, const glm::vec3& lightPos, const glm::vec3& lightColor) {
		unsigned textureID = m_assetID2textureID[material->diffuseTextureAssetID];
		Texture* texture = m_textureRepo->GetTexture(textureID);
		texture->Bind(1);

		unsigned shaderID = m_assetID2shaderID[material->shaderAssetID];
		Shader* shader = m_shaderRepo->GetShader(shaderID);
		shader->Bind();

		shader->SetUniform1i("u_texture", 1);
		shader->SetUniformMat4f("u_mvp", camera->GetMVPMatrix_Perspective(pos));
		shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(rot));

		shader->SetUniform3f("u_modPosition", pos.x, pos.y, pos.z);
		shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
		shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);

		shader->SetUniform1i("u_depthMapTexture", 2);
		glm::mat4 lightSpaceMatrix = m_spotLight->GetLightSpaceMatrix();
		shader->SetUniformMat4f("u_lightSpaceMatrix", lightSpaceMatrix);
		shader->SetUniform1f("u_nearPlane", camera->nearPlane);
		shader->SetUniform1f("u_farPlane", camera->farPlane);

		va->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void PipelineTest::Draw() {
		glfwMakeContextCurrent(window);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}