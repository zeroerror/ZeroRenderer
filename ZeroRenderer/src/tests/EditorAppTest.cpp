#include "EditorAppTest.h"

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

	EditorAppTest::EditorAppTest() {
		editorContext = new EditorContext();
		editorRendererDomain = new EditorRendererDomain();
		editorRendererDomain->Inject(editorContext);

		cameraControllerEnabled = false;
	}

	EditorAppTest::~EditorAppTest() {
		std::cout << "EditorAppTest::~EditorAppTest()" << std::endl;
		GLCall(glfwMakeContextCurrent(window));

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwDestroyWindow(window);
	}

	void EditorAppTest::Init() {
		// ======================== Database
		Database::ImportAssets();
		Database::SetMat_ShaderGUID("asset/material/default.mat", "asset/shader/Default.shader");
		Database::SetMat_DiffuseTextureGUID("asset/material/default.mat", "asset/texture/jerry.png", TEX_SLOT_DIFFUSE_MAP);
		Database::SetMat_ShaderGUID("asset/material/defaultLight.mat", "asset/shader/DefaultLight.shader");
		Database::SetMat_DiffuseTextureGUID("asset/material/defaultLight.mat", "asset/texture/jerry.png", TEX_SLOT_DIFFUSE_MAP);
		Database::SetMat_ShaderGUID("asset/material/depthMap.mat", "asset/shader/DepthMap.shader");
		Database::SetMat_DiffuseTextureGUID("asset/material/depthMap.mat", "asset/texture/jerry.png", TEX_SLOT_DIFFUSE_MAP);
		Database::SetMat_ShaderGUID("asset/material/lightCube.mat", "asset/shader/Default.shader");
		Database::SetMat_DiffuseTextureGUID("asset/material/lightCube.mat", "asset/texture/jerry.png", TEX_SLOT_DIFFUSE_MAP);

		// ======================== GL
		InitOpenGL();
		InitImGui();
		InitShadowMaping();

		// ======================== Load Default Scene
		editorRendererDomain->LoadDefaultScene();
		// ======================== TO BE REMOVE ========================
		scene = editorContext->currentScene;
		cubes = scene->cubes;
		models = scene->models;
		directLight = scene->directLight;
		directLight->shadowType = ShadowType::Hard;
		lightCube = scene->lightCube;
		depthMapImage = scene->depthMapImage;
		sceneCamera = scene->camera;
		cameraController = Camera3DController();
		cameraController.Inject(sceneCamera, window);
		shaderRepo = editorContext->GetShaderRepo();
		textureRepo = editorContext->GetTextureRepo();

		editorContext->sceneViewCamera = sceneCamera;
		editorContext->sceneDirectLight = directLight;
		// ======================== TO BE REMOVE ========================

		// ======================== Device Input - Mouse
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			EditorAppTest* camera3DCubeTest = static_cast<EditorAppTest*>(glfwGetWindowUserPointer(window));
			Transform* camTrans = camera3DCubeTest->scene->camera->transform;
			glm::vec3 pos = camTrans->GetPosition();
			glm::vec3 forward = camTrans->GetForward();
			pos += forward * static_cast<float>(yoffset * camera3DCubeTest->moveSpeed);
			camTrans->SetPosition(pos);
		});
	}

	void EditorAppTest::OnUpdate(const float& deltaTime) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
			cameraControllerEnabled = true;
			cameraController.InitCursorPos();
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			cameraControllerEnabled = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (cameraControllerEnabled) {
			cameraController.Update(deltaTime);
		}

		Camera3D* camera = scene->camera;
		DirectLight* directLight = scene->directLight;
		Cube* lightCube = scene->lightCube;

		camera->Update(deltaTime);

		// - Light Position Control - For Debug
		glm::vec3 forward = -camera->transform->GetForward();
		forward.y = 0;
		forward = glm::normalize(forward);
		glm::vec3 right = glm::cross(glm::vec3(0, -1, 0), forward);

		if (!cameraControllerEnabled) {
			float lightMoveSpeed = 2.0f;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				glm::vec3 pos = directLight->transform->GetPosition();
				pos += forward * deltaTime * lightMoveSpeed;
				directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				glm::vec3 pos = directLight->transform->GetPosition();
				pos -= forward * deltaTime * lightMoveSpeed;
				directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				glm::vec3 pos = directLight->transform->GetPosition();
				pos -= right * deltaTime * lightMoveSpeed;
				directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				glm::vec3 pos = directLight->transform->GetPosition();
				pos += right * deltaTime * lightMoveSpeed;
				directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				glm::vec3 pos = directLight->transform->GetPosition();
				pos.y += deltaTime * lightMoveSpeed;
				directLight->transform->SetPosition(pos);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				glm::vec3 pos = directLight->transform->GetPosition();
				pos.y -= deltaTime * lightMoveSpeed;
				directLight->transform->SetPosition(pos);
			}
			lightCube->transform->SetPosition(directLight->transform->GetPosition());

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
			glm::quat lightRot = directLight->transform->GetRotation();
			lightRot = glm::quat(glm::vec3(0, yawRadius, 0)) * lightRot * glm::quat(glm::vec3(pitchRadius, 0, 0));
			directLight->transform->SetRotation(lightRot);
			lightCube->transform->SetRotation(lightRot);
		}
	}

	void EditorAppTest::OnRender() {
		Repaint();

		DirectLight* directLight = scene->directLight;
		if (directLight->shadowType != ShadowType::None) {
			ShaderMapping();
			Repaint();
		}

		RenderScene();

		if (glfwWindowShouldClose(window)) {
			isClosed = true;
		}
	}

	void EditorAppTest::RenderScene() {
		for (auto cube : *cubes) {
			Material* material = cube->material;
			VertexArray* va = cube->va;
			IndexBuffer* ib = cube->ib;
			glm::vec3 pos = cube->transform->GetPosition();
			glm::quat rot = cube->transform->GetRotation();
			glm::mat4 cameraMVPMatrix = sceneCamera->GetMVPMatrix_Perspective(pos);
			glm::mat4 lightMVPMatrix = directLight->GetMVPMatrix_Perspective(pos);
			RenderObject(material, va, ib, pos, rot, cameraMVPMatrix, lightMVPMatrix);
		}

		// - Light Cube
		Material* material = lightCube->material;
		VertexArray* va = lightCube->va;
		IndexBuffer* ib = lightCube->ib;
		glm::vec3 pos = lightCube->transform->GetPosition();
		glm::quat rot = lightCube->transform->GetRotation();
		glm::mat4 cameraMVPMatrix = sceneCamera->GetMVPMatrix_Perspective(pos);
		RenderObject(material, va, ib, pos, rot, cameraMVPMatrix, cameraMVPMatrix);

		// - Screen Cube 
		material = depthMapImage->material;
		va = depthMapImage->va;
		ib = depthMapImage->ib;
		pos = depthMapImage->transform->GetPosition();
		rot = depthMapImage->transform->GetRotation();
		cameraMVPMatrix = sceneCamera->GetMVPMatrix_Perspective(pos);
		RenderObject(material, va, ib, pos, rot, cameraMVPMatrix, cameraMVPMatrix);

		// - Model
		glm::vec3 lightPos = directLight->transform->GetPosition();
		glm::vec3 lightColor = directLight->color;
		glm::vec3 lightDirection = -directLight->GetLightDirection();

		for (auto model : *models) {
			editorRendererDomain->DrawModel(model);
		}
	}

	void EditorAppTest::ShaderMapping() {
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthTextureRID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "  ########################## Framebuffer is not complete!" << std::endl;
		}
		else {
			for (auto cube : *cubes) {
				Material* material = cube->material;
				VertexArray* va = cube->va;
				IndexBuffer* ib = cube->ib;
				glm::vec3 pos = cube->transform->GetPosition();
				glm::quat rot = cube->transform->GetRotation();
				glm::mat4 lightMVPMatrix = directLight->GetMVPMatrix_Perspective(pos);
				RenderObject(material, va, ib, pos, rot, lightMVPMatrix, lightMVPMatrix);
			}

			// - Model
			for (auto model : *models) {
				Material* material;
				if (editorRendererDomain->TryLoadMaterialByAssetPath("asset/material/default.mat", material)) {
					editorRendererDomain->DrawModel(model, material);
				}
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, scene->camera->scrWidth, scene->camera->scrHeight);
	}

	void EditorAppTest::RenderObject(Material* material, VertexArray* va, IndexBuffer* ib, const glm::vec3& pos, const glm::quat& rot, const glm::mat4& cameraMVPMatrix, const glm::mat4& lightMVPMatrix) {
		if (material != nullptr) {
			Shader* shader = material->shader;
			if (shader != nullptr) {
				shader->Bind();
				shader->SetUniform1i("u_diffuseMap", TEX_SLOT_DIFFUSE_MAP);
				shader->SetUniform1i("u_depthMap", TEX_SLOT_DEPTH_MAP);
				shader->SetUniform1i("u_specularMap", 0);
				shader->SetUniformMat4f("u_mvp", cameraMVPMatrix);
				shader->SetUniformMat4f("u_modRotationMatrix", glm::toMat4(rot));
				shader->SetUniform3f("u_modPosition", pos.x, pos.y, pos.z);

				glm::vec3 lightPos = directLight->transform->GetPosition();
				glm::vec3 lightColor = directLight->color;
				glm::vec3 lightDirection = -directLight->GetLightDirection();
				shader->SetUniform3f("u_lightPosition", lightPos.x, lightPos.y, lightPos.z);
				shader->SetUniform3f("u_lightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
				shader->SetUniform3f("u_lightColor", lightColor.x, lightColor.y, lightColor.z);

				shader->SetUniformMat4f("u_lightMVPMatrix", lightMVPMatrix);
				shader->SetUniform1f("u_nearPlane", sceneCamera->nearPlane);
				shader->SetUniform1f("u_farPlane", sceneCamera->farPlane);
			}

			Texture* texture = material->diffuseTexture;
			if (texture != nullptr) texture->Bind(TEX_SLOT_DIFFUSE_MAP);
			glActiveTexture(GL_TEXTURE0 + TEX_SLOT_DEPTH_MAP);
			glBindTexture(GL_TEXTURE_2D, depthTextureRID);
		}

		va->Bind();
		ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void EditorAppTest::Draw() {
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	void EditorAppTest::OnImGuiRender() {
		ImGui::SetCurrentContext(imguiContext);
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Light Menu");
		glm::vec3 lightColor = directLight->color;
		ImGui::SliderFloat3("Spot Light Color", &lightColor.x, 0.0f, 1.0f);
		directLight->color = lightColor;
		ImGui::End();

		ImGui::Begin("Camera Controller Menu");
		ImGui::SliderFloat("Camera Move Speed", &cameraController.moveSpeed, 0.0f, 1.0f);
		ImGui::SliderFloat("Camera Rotate Speed", &cameraController.rotateSpeed, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Begin("Model Menu");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		Draw();
	}

	void EditorAppTest::InitOpenGL() {
		if (!glfwInit())
			return;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(1920, 1080, "Assimp Test Window", NULL, NULL);
		if (!window) {
			GLCall(glfwTerminate());
			return;
		}

		GLCall(glfwMakeContextCurrent(window));

		// Enable VSync
		GLCall(glfwSwapInterval(1));

		if (glewInit() != GLEW_OK)
			std::cout << "ERROR" << std::endl;

		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void EditorAppTest::InitShadowMaping() {
		shadowMapWidth = 2048;
		shadowMapHeight = 2048;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &depthTextureRID);
		glActiveTexture(GL_TEXTURE0 + TEX_SLOT_DEPTH_MAP);
		glBindTexture(GL_TEXTURE_2D, depthTextureRID);

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureRID, 0));
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void EditorAppTest::InitImGui() {
		IMGUI_CHECKVERSION();
		imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(imguiContext);

		ImGui::StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void EditorAppTest::Repaint() {
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