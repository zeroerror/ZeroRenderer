#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Test.h"
#include "Camera3D.h"
#include "Camera3DController.h"
#include "Cube.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"

namespace test {

	class PipelineTest :public Test {
	public:
		Camera3D camera;
		GLFWwindow* window;
		const float moveSpeed = 0.1f;
		const float rotateSpeed = 0.1f;

		PipelineTest();
		~PipelineTest();

		void Init();
		void ProcessInput();
		void OnUpdate(const float& deltaTime) override;
		void OnRender() override;

	private:
		int m_screen_width;
		int m_screen_height;
		Cube* m_cubes[10];
		Camera3DController m_cameraController;
		ShaderRepo* m_shaderRepo;
		TextureRepo* m_textureRepo;
		std::unordered_map<std::string, unsigned int> m_assetPath2AssetID;
		std::unordered_map<unsigned int, std::string> m_assetID2AssetPath;
		std::unordered_map<unsigned int, unsigned int> m_assetID2shaderID;
		std::unordered_map<unsigned int, unsigned int> m_assetID2textureID;
		std::unordered_map<std::string, unsigned int> m_assetPath2shaderID;
		std::unordered_map<std::string, unsigned int> m_assetPath2textureID;

		void LoadAssetsDatabase();
		void CreateWindow();
		void LoadCamera();
		void LoadShaders();
		void LoadTextures();

		void SetGL();
		void RenderObject();
		void CallGL();

	};

}