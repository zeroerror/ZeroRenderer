#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Test.h"
#include "Camera3D.h"
#include "Camera3DController.h"
#include "Cube.h"
#include "Rectangle.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "Material.h"
#include "SpotLight.h"
#include <DirectLight.h>

namespace test {

	class PipelineTest:public Test {
	public:
		Camera3D* camera;
		GLFWwindow* window;
		ImGuiContext* imguiContext;
		const float moveSpeed = 0.1f;
		const float rotateSpeed = 0.1f;

		PipelineTest();
		~PipelineTest();

		void Init();
		void OnUpdate(const float& deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unordered_map<std::string, unsigned int> m_assetPath2AssetID;
		std::unordered_map<unsigned int, std::string> m_assetID2AssetPath;
		std::unordered_map<unsigned int, unsigned int> m_assetID2shaderID;
		std::unordered_map<unsigned int, unsigned int> m_assetID2textureID;
		std::unordered_map<std::string, unsigned int> m_assetPath2shaderID;
		std::unordered_map<std::string, unsigned int> m_assetPath2textureID;

		int m_shadowMapWidth;
		int m_shadowMapHeight;
		int m_screen_width;
		int m_screen_height;

		Camera3DController m_cameraController;
		bool m_cameraControllerEnabled;

		std::vector<Cube*> m_cubes;
		Cube* m_lightCube;
		Rectangle* m_depthMapImage;
		ShaderRepo* m_shaderRepo;
		TextureRepo* m_textureRepo;
		DirectLight* m_directLight;
		GLuint m_depthTexture;
		GLuint m_framebuffer;

		void LoadAssetsDatabase();
		void InitOpenGL();
		void InitImGui();
		void LoadCamera();
		void LoadShaders();
		void LoadTextures();

		void Repaint();
		void RenderScene();
		void RenderObject(Material* material, VertexArray* va, IndexBuffer* ib, const glm::vec3& pos, const glm::quat& rot,const glm::mat4& cameraMVPMatrix,const glm::mat4& lightMVPMatrix);
		void RenderObjectForDepthMap();
		void RenderSceneShadowMap();
		void Draw();

	};

}