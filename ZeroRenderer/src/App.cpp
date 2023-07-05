#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image/stb_image.h>
#include <stack>

#include "Database.h"

// ********************** EDITOR USER CONFIG **********************
const int EDITOR_WINDOW_WIDTH = 1920;
const int EDITOR_WINDOW_HEIGHT = 1080;

const ImVec2 EDITOR_WINDOW_PROJECT_POSITION = ImVec2(0, 0);
const int EDITOR_WINDOW_PROJECT_WIDTH = EDITOR_WINDOW_WIDTH * 0.25f;
const int EDITOR_WINDOW_PROJECT_HEIGHT = EDITOR_WINDOW_HEIGHT;

const ImVec2 EDITOR_WINDOW_PROJECT_DETAILS_POSITION = ImVec2(EDITOR_WINDOW_PROJECT_WIDTH, 0);
const int EDITOR_WINDOW_PROJECT_DETAILS_WIDTH = EDITOR_WINDOW_WIDTH * 0.75;
const int EDITOR_WINDOW_PROJECT_DETAILS_HEIGHT = EDITOR_WINDOW_HEIGHT;

// ********************** EDITOR CACHE **********************
AssetTreeNode* _rootNode;
static string _curProjectChoosedPath = "";
static string _curProjectDetailsChoosedPath = "";
double _assetClickTime;
unsigned int _texture_id;

void ImGui_ShowProjectPanel(AssetTreeNode* node, string dir, float xOffset) {

	// Self GUI
	ImGui::Indent(xOffset);
	ImGui::Image(reinterpret_cast<ImTextureID>(_texture_id), ImVec2(32, 32)); // 调整图标大小
	ImGui::SameLine();

	// Listen double click
	string assetName = node->assetName;
	const char* assetNamec = assetName.c_str();
	string choosedPath = dir + assetName;
	if (ImGui::Selectable(assetNamec, choosedPath == _curProjectChoosedPath)) {
		_curProjectChoosedPath = choosedPath;
		double nowClickTime = glfwGetTime();
		double clickTimeOffset = nowClickTime - _assetClickTime;
		if (clickTimeOffset < 0.2f) {
			node->isExpanded = !node->isExpanded;
		}
		_assetClickTime = nowClickTime;
	}

	if (!node->isExpanded) {
		ImGui::Unindent(xOffset);
		return;
	}

	// Child
	choosedPath += "/";
	for (auto kvp : node->childNodes) {
		AssetTreeNode* childNode = kvp.second;
		ImGui_ShowProjectPanel(childNode, dir, xOffset + 10.0f);
	}
}

void ImGui_ShowProjectPanel() {
	float xoffset = 0;
	ImGui_ShowProjectPanel(_rootNode, "", xoffset);
}

void ImGui_ShowProjectDetailsPanel(const AssetTreeNode* node) {
	for (auto kvp : node->childNodes) {
		AssetTreeNode* node = kvp.second;
		if (node->isDir) {
			ImGui::Image(reinterpret_cast<ImTextureID>(_texture_id), ImVec2(64, 64)); // 调整图标大小
		}
		const char* assetName = node->assetName.c_str();
		if (ImGui::Selectable(assetName, assetName == _curProjectDetailsChoosedPath)) {
			_curProjectDetailsChoosedPath = assetName;
		}
	}
}

int main() {

	// Import Database
	Database::ImportAssets();
	_rootNode = Database::GetRootAssetTreeNode();
	_rootNode->isExpanded = true;

	// Initialize GLFW and create a window
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(EDITOR_WINDOW_WIDTH, EDITOR_WINDOW_HEIGHT, "Zero Engine v0.0.1", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	// Initialize ImGui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// 创建图像纹理
	string path = "asset/texture/folder.png";
	int width, height, channels;
	unsigned char* imgData = stbi_load(path.c_str(), &width, &height, &channels, 4);
	glGenTextures(1, &_texture_id);
	glBindTexture(GL_TEXTURE_2D, _texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// - Project Panel
		ImGui::SetNextWindowPos(EDITOR_WINDOW_PROJECT_POSITION);
		ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_PROJECT_WIDTH, EDITOR_WINDOW_PROJECT_HEIGHT));
		ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		ImGui_ShowProjectPanel();
		ImGui::End();

		// - Project Details Panel 
		ImGui::SetNextWindowPos(EDITOR_WINDOW_PROJECT_DETAILS_POSITION);
		ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_PROJECT_DETAILS_WIDTH, EDITOR_WINDOW_PROJECT_DETAILS_HEIGHT));
		ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		if (_curProjectChoosedPath != "") {
			AssetTreeNode* node;
			if (_rootNode->TryGetNodeByPath(_curProjectChoosedPath, node)) {
				ImGui_ShowProjectDetailsPanel(node);
			}
		}
		ImGui::End();

		// Rendering ImGui
		ImGui::Render();
		glViewport(0, 0, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}