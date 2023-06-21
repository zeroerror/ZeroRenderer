#include "AssimpTest.h"

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GLDebug.h"
#include "Camera3DController.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "SpotLight.h"
#include "ShadowType.h"
#include "CameraType.h"
#include "Database.h"

namespace test {

	AssimpTest::AssimpTest() {
		m_assetPath2AssetID = std::unordered_map<std::string, unsigned int>();
		m_assetID2AssetPath = std::unordered_map<unsigned int, std::string>();

		m_assetID2shaderID = std::unordered_map<unsigned int, unsigned int>();
		m_assetID2textureID = std::unordered_map<unsigned int, unsigned int>();

		m_assetPath2shaderID = std::unordered_map<std::string, unsigned int>();
		m_assetPath2textureID = std::unordered_map<std::string, unsigned int>();

		m_shaderRepo = new ShaderRepo();
		m_textureRepo = new TextureRepo();

		m_models = std::vector<Model*>();

		m_cameraControllerEnabled = false;

		indiceDrawCount = 100000;
	}

	AssimpTest::~AssimpTest() {
		std::cout << "AssimpTest::~AssimpTest()" << std::endl;
		GLCall(glfwMakeContextCurrent(window));

		for (auto cube : m_cubes) {
			delete cube;
		}

		delete m_lightCube;
		delete m_depthMapImage;
		delete m_shaderRepo;
		delete m_textureRepo;
		delete m_directLight;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwDestroyWindow(window);
	}

	void AssimpTest::Init() {
		m_screen_width = 1920;
		m_screen_height = 1080;
		m_shadowMapWidth = 2048;
		m_shadowMapHeight = 2048;

		LoadAssetsDatabase();
		InitOpenGL();
		InitImGui();
		LoadCamera();
		LoadShaders();
		LoadTextures();

		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			AssimpTest* camera3DCubeTest = static_cast<AssimpTest*>(glfwGetWindowUserPointer(window));
			Transform* camTrans = camera3DCubeTest->camera->transform;
			glm::vec3 pos = camTrans->GetPosition();
			glm::vec3 forward = camTrans->GetForward();
			pos += forward * static_cast<float>(yoffset * camera3DCubeTest->moveSpeed);
			camTrans->SetPosition(pos);
		});

		// ======================== Database
		Database::LoadDatabase();

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

		// Light 
		m_directLight = new DirectLight();
		m_directLight->shadowType = ShadowType::Hard;
		m_directLight->transform->SetPosition(camera->transform->GetPosition());
		m_directLight->transform->SetRotation(camera->transform->GetRotation());
		m_directLight->fov = camera->fov;
		m_directLight->scrWidth = camera->scrWidth;
		m_directLight->scrHeight = camera->scrHeight;
		m_directLight->nearPlane = camera->nearPlane;
		m_directLight->farPlane = camera->farPlane;
		m_directLight->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		// Create a central light source cube
		m_lightCube = Cube::CreateCube(0.2f, 0.2f, 1.0f);
		m_lightCube->material = lightCubeMaterial;

		// Create a depth map 2D image
		m_depthMapImage = Rectangle::CreateRectangle(16.0f, 9.0f);
		m_depthMapImage->transform->SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));
		m_depthMapImage->transform->SetRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
		m_depthMapImage->material = depthMapMaterial;

		// Create the ground
		Cube* groundCube = Cube::CreateCube(20.0f, 0.1f, 30.0f);
		groundCube->transform->SetPosition(glm::vec3(0.0f, -0.05f, 0.0f));
		groundCube->material = defaultLightMaterial;
		m_cubes.push_back(groundCube);

		// Create walls
		Cube* wall1 = Cube::CreateCube(1.0f, 5.0f, 10.0f);
		wall1->transform->SetPosition(glm::vec3(-8.0f, 2.5f, 0.0f));
		wall1->material = defaultLightMaterial;
		m_cubes.push_back(wall1);

		Cube* wall2 = Cube::CreateCube(10.0f, 5.0f, 1.0f);
		wall2->transform->SetPosition(glm::vec3(0.0f, 2.5f, -8.0f));
		wall2->material = defaultLightMaterial;
		m_cubes.push_back(wall2);

		// Create obstacles
		Cube* obstacle1 = Cube::CreateCube(2.0f, 2.0f, 2.0f);
		obstacle1->transform->SetPosition(glm::vec3(-4.0f, 1.0f, 4.0f));
		obstacle1->material = defaultLightMaterial;
		m_cubes.push_back(obstacle1);

		Cube* obstacle2 = Cube::CreateCube(2.0f, 2.0f, 2.0f);
		obstacle2->transform->SetPosition(glm::vec3(4.0f, 1.0f, -4.0f));
		obstacle2->material = defaultLightMaterial;
		m_cubes.push_back(obstacle2);

		Cube* obstacle3 = Cube::CreateCube(3.0f, 1.0f, 2.0f);
		obstacle3->transform->SetPosition(glm::vec3(6.0f, 0.5f, 6.0f));
		obstacle3->material = defaultLightMaterial;
		m_cubes.push_back(obstacle3);

		// ========================== Load Model
		Model* model = new Model();
		model->LoadModel("asset/model/nanosuit/nanosuit.obj");
		model->transform->SetPosition(glm::vec3(10.0f, 0.0f, 20.0f));
		model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
		model->material = defaultLightMaterial;
		m_models.push_back(model);

		model = new Model();
		model->LoadModel("asset/model/nanosuit/nanosuit.obj");
		model->transform->SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));
		model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
		model->material = defaultLightMaterial;
		m_models.push_back(model);

		model = new Model();
		model->LoadModel("asset/model/nanosuit/nanosuit.obj");
		model->transform->SetPosition(glm::vec3(-10.0f, 0.0f, 20.0f));
		model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
		model->material = defaultLightMaterial;
		m_models.push_back(model);
	}

	void AssimpTest::OnUpdate(const float& deltaTime) {
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
				glm::vec3 pos = m_directLight->transform->GetPosition();
				pos += forward * deltaTime * lightMoveSpeed;
				m_directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				glm::vec3 pos = m_directLight->transform->GetPosition();
				pos -= forward * deltaTime * lightMoveSpeed;
				m_directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				glm::vec3 pos = m_directLight->transform->GetPosition();
				pos -= right * deltaTime * lightMoveSpeed;
				m_directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				glm::vec3 pos = m_directLight->transform->GetPosition();
				pos += right * deltaTime * lightMoveSpeed;
				m_directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				glm::vec3 pos = m_directLight->transform->GetPosition();
				pos.y += deltaTime * lightMoveSpeed;
				m_directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				glm::vec3 pos = m_directLight->transform->GetPosition();
				pos.y -= deltaTime * lightMoveSpeed;
				m_directLight->transform->SetPosition(pos);
			}
			m_lightCube->transform->SetPosition(m_directLight->transform->GetPosition());

			// - Light Rotation Control - For Debug
			float rotateX = 0;
			float rotateY = 0;
			float lightRotateSpeed = 10.0f;
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				rotateY -= lightRotateSpeed * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				rotateY += lightRotateSpeed * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
				rotateX -= lightRotateSpeed * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
				rotateX += lightRotateSpeed * deltaTime;
			}
			float yawRadius = -glm::radians(rotateX);
			float pitchRadius = -glm::radians(rotateY);
			glm::quat lightRot = m_directLight->transform->GetRotation();
			lightRot = glm::quat(glm::vec3(0, yawRadius, 0)) * lightRot * glm::quat(glm::vec3(pitchRadius, 0, 0));
			m_directLight->transform->SetRotation(lightRot);
			m_lightCube->transform->SetRotation(lightRot);
		}
	}

	void AssimpTest::OnRender() {
		Repaint();

		if (m_directLight->shadowType != ShadowType::None) {
			RenderSceneShadowMap();
			Repaint();
		}

		RenderScene();

		if (glfwWindowShouldClose(window)) {
			isClosed = true;
		}
	}

	void AssimpTest::RenderScene() {
		for (auto cube : m_cubes) {
			Material* material = cube->material;
			VertexArray* va = cube->va;
			IndexBuffer* ib = cube->ib;
			glm::vec3 pos = cube->transform->GetPosition();
			glm::quat rot = cube->transform->GetRotation();
			glm::mat4 cameraMVPMatrix = camera->GetMVPMatrix_Perspective(pos);
			glm::mat4 lightMVPMatrix = m_directLight->GetMVPMatrix_Perspective(pos);
			RenderObject(material, va, ib, pos, rot, cameraMVPMatrix, lightMVPMatrix);
		}

		// - Light Cube
		Material* material = m_lightCube->material;
		VertexArray* va = m_lightCube->va;
		IndexBuffer* ib = m_lightCube->ib;
		glm::vec3 pos = m_lightCube->transform->GetPosition();
		glm::quat rot = m_lightCube->transform->GetRotation();
		glm::mat4 cameraMVPMatrix = camera->GetMVPMatrix_Perspective(pos);
		RenderObject(material, va, ib, pos, rot, cameraMVPMatrix, cameraMVPMatrix);

		// - Screen Cube 
		material = m_depthMapImage->material;
		va = m_depthMapImage->va;
		ib = m_depthMapImage->ib;
		pos = m_depthMapImage->transform->GetPosition();
		rot = m_depthMapImage->transform->GetRotation();
		cameraMVPMatrix = camera->GetMVPMatrix_Perspective(pos);
		RenderObject(material, va, ib, pos, rot, cameraMVPMatrix, cameraMVPMatrix);

		// - Model
		Shader* shader = m_shaderRepo->GetShader(m_assetID2shaderID[1000]);
		shader->Bind();
		for (auto model : m_models) {
			shader->SetUniformMat4f("u_mvp", camera->GetMVPMatrix_Perspective(model->transform->GetPosition()));
			shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(model->transform->GetRotation()));
			model->RenderModel();
		}
	}

	void AssimpTest::RenderSceneShadowMap() {
		glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "########################## Framebuffer is not complete!" << std::endl;
		}
		else {
			RenderObjectForDepthMap();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_screen_width, m_screen_height);
	}

	void AssimpTest::RenderObjectForDepthMap() {
		for (auto cube : m_cubes) {
			Material* material = cube->material;
			VertexArray* va = cube->va;
			IndexBuffer* ib = cube->ib;
			glm::vec3 pos = cube->transform->GetPosition();
			glm::quat rot = cube->transform->GetRotation();
			glm::mat4 lightMVPMatrix = m_directLight->GetMVPMatrix_Perspective(pos);
			RenderObject(material, va, ib, pos, rot, lightMVPMatrix, lightMVPMatrix);
		}

		// - Model
		for (auto model : m_models) {
			glm::vec3 pos = model->transform->GetPosition();
			glm::quat rot= model->transform->GetRotation();
			glm::mat4 lightMVPMatrix = m_directLight->GetMVPMatrix_Perspective(pos);
			unsigned shaderID = m_assetID2shaderID[model->material->shaderAssetID];
			Shader* shader = m_shaderRepo->GetShader(shaderID);
			shader->Bind();
			shader->SetUniform1i("u_texture", 1);
			shader->SetUniformMat4f("u_mvp", lightMVPMatrix);
			shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(rot));
			shader->SetUniform3f("u_modPosition", pos.x, pos.y, pos.z);

			glm::vec3 lightPos = m_directLight->transform->GetPosition();
			glm::vec3 lightColor = m_directLight->color;
			glm::vec3 lightDirection = -m_directLight->GetLightDirection();
			shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
			shader->SetUniform3f("u_lightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
			shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);
			shader->SetUniformMat4f("u_lightMVPMatrix", lightMVPMatrix);
			shader->SetUniform1f("u_nearPlane", camera->nearPlane);
			shader->SetUniform1f("u_farPlane", camera->farPlane);

			model->RenderModel();
		}
	}

	void AssimpTest::RenderObject(Material* material, VertexArray* va, IndexBuffer* ib, const glm::vec3& pos, const glm::quat& rot, const glm::mat4& cameraMVPMatrix, const glm::mat4& lightMVPMatrix) {
		unsigned textureID = m_assetID2textureID[material->diffuseTextureAssetID];
		Texture* texture = m_textureRepo->GetTexture(textureID);
		texture->Bind(1);

		unsigned shaderID = m_assetID2shaderID[material->shaderAssetID];
		Shader* shader = m_shaderRepo->GetShader(shaderID);
		shader->Bind();

		shader->SetUniform1i("u_texture", 1);
		shader->SetUniformMat4f("u_mvp", cameraMVPMatrix);
		shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(rot));
		shader->SetUniform3f("u_modPosition", pos.x, pos.y, pos.z);

		glm::vec3 lightPos = m_directLight->transform->GetPosition();
		glm::vec3 lightColor = m_directLight->color;
		glm::vec3 lightDirection = -m_directLight->GetLightDirection();
		shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
		shader->SetUniform3f("u_lightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
		shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);

		shader->SetUniform1i("u_depthMapTexture", 2);
		shader->SetUniformMat4f("u_lightMVPMatrix", lightMVPMatrix);
		shader->SetUniform1f("u_nearPlane", camera->nearPlane);
		shader->SetUniform1f("u_farPlane", camera->farPlane);

		va->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void AssimpTest::Draw() {
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	void AssimpTest::OnImGuiRender() {
		ImGui::SetCurrentContext(imguiContext);
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Light Menu");
		glm::vec3 lightColor = m_directLight->color;
		ImGui::SliderFloat3("Spot Light Color", &lightColor.x, 0.0f, 1.0f);
		m_directLight->color = lightColor;
		ImGui::End();

		ImGui::Begin("Camera Controller Menu");
		ImGui::SliderFloat("Camera Move Speed", &m_cameraController.moveSpeed, 0.0f, 1.0f);
		ImGui::SliderFloat("Camera Rotate Speed", &m_cameraController.rotateSpeed, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Begin("Model Menu");
		int count = indiceDrawCount;
		ImGui::SliderInt("Indice Draw Count", &count, 0, 10'0000);
		indiceDrawCount = count;
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		Draw();
	}

	void AssimpTest::LoadAssetsDatabase() {
		m_assetPath2AssetID.insert(std::pair<std::string, unsigned int>("asset/Textuasset/jerry.png", 1000));
		m_assetPath2AssetID.insert(std::pair<std::string, unsigned int>("asset/Textuasset/room.png", 2000));

		m_assetID2AssetPath.insert(std::pair<unsigned int, std::string>(1000, "asset/Textuasset/jerry.png"));
		m_assetID2AssetPath.insert(std::pair<unsigned int, std::string>(2000, "asset/Textuasset/room.png"));
	}

	void AssimpTest::InitOpenGL() {
		/* Initialize the library */
		if (!glfwInit())
			return;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(m_screen_width, m_screen_height, "Assimp Test Window", NULL, NULL);
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

		// ======================= Create Shadow Mapping
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowMapWidth, m_shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void AssimpTest::InitImGui() {
		IMGUI_CHECKVERSION();
		imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(imguiContext);

		ImGui::StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void AssimpTest::LoadCamera() {
		camera = new Camera3D();
		camera->scrWidth = m_screen_width;
		camera->scrHeight = m_screen_height;
		camera->transform->SetPosition(glm::vec3(0, 10, -10));
		camera->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f))));
		m_cameraController = Camera3DController();
		m_cameraController.Inject(camera, window);
	}

	void AssimpTest::LoadShaders() {
		unsigned int shaderID = m_shaderRepo->LoadShader("asset/shader/Default.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(1000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("asset/shader/Default.shader", shaderID));

		shaderID = m_shaderRepo->LoadShader("asset/shader/DefaultLight.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(2000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("asset/shader/DefaultLight.shader", shaderID));

		shaderID = m_shaderRepo->LoadShader("asset/shader/DepthMap.shader");
		m_assetID2shaderID.insert(std::pair<unsigned int, unsigned int>(3000, shaderID));
		m_assetPath2shaderID.insert(std::pair<std::string, unsigned int>("asset/shader/DepthMap.shader", shaderID));
	}

	void AssimpTest::LoadTextures() {
		unsigned int textureID = m_textureRepo->LoadTexture("asset/Textuasset/jerry.png");
		m_assetID2textureID.insert(std::pair<unsigned int, unsigned int>(1000, textureID));
		m_assetPath2textureID.insert(std::pair<std::string, unsigned int>("asset/Textuasset/jerry.png", textureID));

		textureID = m_textureRepo->LoadTexture("asset/Textuasset/room.png");
		m_assetID2textureID.insert(std::pair<unsigned int, unsigned int>(2000, textureID));
		m_assetPath2textureID.insert(std::pair<std::string, unsigned int>("asset/Textuasset/room.png", textureID));
	}

	void AssimpTest::Repaint() {
		glfwMakeContextCurrent(window);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}