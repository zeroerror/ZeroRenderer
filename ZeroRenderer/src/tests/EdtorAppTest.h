#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Test.h"
#include "Camera3D.h"
#include "Camera3DController.h"
#include "Cube.h"
#include "Rectangle.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "Material.h"
#include "SpotLight.h"
#include "DirectLight.h"
#include "Mesh.h"
#include "Model.h"
#include "Scene.h"
#include "EditorContext.h"
#include "EditorRendererDomain.h"

namespace test {

	class EdtorAppTest:public Test {
	public:
		// ---- Scene
		Scene* scene;
		// ---- Scene(TO BE REMOVE)
		std::vector<Cube*>* cubes;
		std::vector<Model*>* models;
		Rectangle* depthMapImage;
		Cube* lightCube;
		Camera3D* sceneCamera;
		DirectLight* directLight;

		GLFWwindow* window;
		ImGuiContext* imguiContext;

		int shadowMapWidth;
		int shadowMapHeight;

		Camera3DController cameraController;
		bool cameraControllerEnabled;
		GLuint depthTextureRID;
		GLuint framebuffer;

		const float moveSpeed = 0.1f;
		const float rotateSpeed = 0.1f;

		// ---- Editor
		EditorContext* editorContext;
		EditorRendererDomain* editorRendererDomain;
		// ---- Editor(TO BE REMOVE)
		ShaderRepo* shaderRepo;
		TextureRepo* textureRepo;

		EdtorAppTest();
		~EdtorAppTest();

		void Init();
		void OnUpdate(const float& deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		
		void InitOpenGL();
		void InitImGui();
		void InitShadowMaping();

		void Repaint();
		void RenderScene();
		void RenderObject(Material* material, VertexArray* va, IndexBuffer* ib, const glm::vec3& pos, const glm::quat& rot, const glm::mat4& cameraMVPMatrix, const glm::mat4& lightMVPMatrix);
		void ShaderMapping();
		void Draw();
	};

}