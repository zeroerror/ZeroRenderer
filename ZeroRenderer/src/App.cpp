#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image/stb_image.h>
#include <stack>
#include <src/editor/Scene.h>

#include "Database.h"
#include "Serialization.h"
#include "EditorRendererDomain.h"

using namespace Serialization;
using namespace glm;

// ********************** EDITOR Context **********************
EditorContext* editorContext;
EditorRendererDomain* editorRendererDomain;

// ********************** EDITOR USER CONFIG **********************
const int EDITOR_WINDOW_WIDTH = 1920;
const int EDITOR_WINDOW_HEIGHT = 1080;

const ImVec2 EDITOR_WINDOW_PROJECT_POSITION = ImVec2(0, EDITOR_WINDOW_HEIGHT * (1.8f / 3.0f));
const int EDITOR_WINDOW_PROJECT_WIDTH = EDITOR_WINDOW_WIDTH;
const int EDITOR_WINDOW_PROJECT_HEIGHT = EDITOR_WINDOW_HEIGHT / 3.0f;
const int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH = EDITOR_WINDOW_PROJECT_WIDTH / 4.0f;
const int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT = EDITOR_WINDOW_PROJECT_HEIGHT;
const int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH = EDITOR_WINDOW_WIDTH - EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
const int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT = EDITOR_WINDOW_PROJECT_HEIGHT;

enum EditorPanelFlags_ {
	EditorPanelFlags_None = 0,
	EditorPanelFlags_ProjectLeftColunm = 1 << 0,
	EditorPanelFlags_ProjectRightColunm = 1 << 1,
};

#pragma region [EDITOR CACHE]

AssetTreeNode* _rootNode = nullptr;
AssetTreeNode* _curProjectChoosedNode = nullptr;
AssetTreeNode* _curProjectDetailsChoosedNode = nullptr;
EditorPanelFlags_ _curChoosedPanelFlags = EditorPanelFlags_None;
double _assetClickTime;
unsigned int _texture_id;

#pragma endregion

#pragma region [Device Input]

enum KeyStatus_ {
	KeyStatus_None = 0,
	KeyStatus_Up = 1,
	KeyStatus_Down = 2,
	KeyStatus_Pressing = 3,
};

KeyStatus_ _allKeyStatus[1024] = {};

enum MouseButtons_ {
	MouseButtons_None = -1,
	MouseButtons_Left = 0,
	MouseButtons_Right = 1,
	MouseButtons_Middle = 2,
};

bool _CheckMouseDown(const MouseButtons_& button) {
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseDown[button];
}

bool _CheckKeyDown(const ImGuiKey& key) {
	ImGuiIO& io = ImGui::GetIO();
	return io.KeysDown[key];
}

bool _IsInAABB(const vec2& v, const ImVec2& min, const ImVec2& max) {
	return v.x > min.x && v.x < max.x&& v.y>min.y && v.y < max.y;
}

int _Clamp(const int& v, const int& min, const int& max) {
	return v<min ? min : v>max ? max : v;
}

vec2 GetMousePos() {
	ImVec2 p = ImGui::GetMousePos();
	return vec2(p.x, p.y);
}

bool GetMouseButtonDown(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Down; }
bool GetMouseButtonPressing(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Pressing; }
bool GetMouseButtonUp(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Up; }
bool GetKeyDown(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Down; }
bool GetKeyPressing(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Pressing; }
bool GetKeyUp(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Up; }
void TickInput() {
	if (_CheckMouseDown(MouseButtons_Left))_allKeyStatus[MouseButtons_Left] = (KeyStatus_)_Clamp((_allKeyStatus[MouseButtons_Left] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[MouseButtons_Left] = (KeyStatus_)_Clamp((_allKeyStatus[MouseButtons_Left] - 1), KeyStatus_None, KeyStatus_Up);
	if (_CheckMouseDown(MouseButtons_Right))_allKeyStatus[MouseButtons_Right] = (KeyStatus_)_Clamp((_allKeyStatus[MouseButtons_Right] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[MouseButtons_Right] = (KeyStatus_)_Clamp((_allKeyStatus[MouseButtons_Right] - 1), KeyStatus_None, KeyStatus_Up);
	if (_CheckMouseDown(MouseButtons_Middle))_allKeyStatus[MouseButtons_Middle] = (KeyStatus_)_Clamp((_allKeyStatus[MouseButtons_Middle] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[MouseButtons_Middle] = (KeyStatus_)_Clamp((_allKeyStatus[MouseButtons_Middle] - 1), KeyStatus_None, KeyStatus_Up);

	if (_CheckKeyDown(ImGuiKey_Backspace))_allKeyStatus[ImGuiKey_Backspace] = (KeyStatus_)_Clamp((_allKeyStatus[ImGuiKey_Backspace] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[ImGuiKey_Backspace] = (KeyStatus_)_Clamp((_allKeyStatus[ImGuiKey_Backspace] - 1), KeyStatus_None, KeyStatus_Up);
}

#pragma endregion

#pragma region [Project Panel]

void ImGui_ShowProjectPanel(AssetTreeNode* node, string dir, float xOffset) {
	// Self GUI
	ImGui::Indent(xOffset);
	if (node->isDir) {
		ImGui::Image(reinterpret_cast<ImTextureID>(_texture_id), ImVec2(32, 32)); // 调整图标大小
		ImGui::SameLine();
	}
	string assetName = node->assetName;
	const char* assetNamec = assetName.c_str();
	string choosedPath = dir + assetName;
	ImGui::PushID(1);
	if (ImGui::Selectable(assetNamec, _curProjectChoosedNode != nullptr ? choosedPath == _curProjectChoosedNode->assetPath : false)) {
		double nowClickTime = glfwGetTime();
		double clickTimeOffset = nowClickTime - _assetClickTime;
		_assetClickTime = nowClickTime;

		if (node->isDir) {
			_curProjectChoosedNode = node;
			if (clickTimeOffset < 0.2f) {
				node->isExpanded = !node->isExpanded;
			}
		}
	}
	ImGui::PopID();

	if (!node->isExpanded) {
		ImGui::Unindent(xOffset);
		return;
	}

	// Child
	xOffset += 10.0f;
	choosedPath += "/";
	for (auto kvp : node->childNodes) {
		AssetTreeNode* childNode = kvp.second;
		ImGui_ShowProjectPanel(childNode, choosedPath, xOffset);
	}
}

void ImGui_ShowProjectPanel() {
	float xoffset = 0;
	ImGui_ShowProjectPanel(_rootNode, "", xoffset);
}

void ImGui_ShowProjectDetailsPanel(const AssetTreeNode* node) {
	ImGui::Text(_curProjectChoosedNode->assetPath.c_str());
	ImGui::Spacing();
	ImGui::Indent(10.0f);
	unsigned int colunmCount = EDITOR_WINDOW_PROJECT_HEIGHT;
	ImGui::PushID(2);
	for (auto kvp : node->childNodes) {
		AssetTreeNode* node = kvp.second;
		if (node->isDir) {
			ImGui::Image(reinterpret_cast<ImTextureID>(_texture_id), ImVec2(32, 32));
			ImGui::SameLine();
		}
		const char* assetNamec = node->assetName.c_str();
		if (ImGui::Selectable(assetNamec, _curProjectDetailsChoosedNode != nullptr ? assetNamec == _curProjectDetailsChoosedNode->assetPath : false, ImGuiSelectableFlags_None, ImVec2(0, 0))) {
			_curProjectDetailsChoosedNode = node;
			double nowClickTime = glfwGetTime();
			double clickTimeOffset = nowClickTime - _assetClickTime;
			_assetClickTime = nowClickTime;
			if (clickTimeOffset < 0.2f) {
				AssetTreeNode* detailNode;
				if (_curProjectDetailsChoosedNode->isDir) {
					_curProjectChoosedNode = _curProjectDetailsChoosedNode;
					_curProjectDetailsChoosedNode->isExpanded = true;
				}
			}
		}
	}
	ImGui::PopID();
}
#pragma endregion
















int main() {
	// Import Database
	Database::ImportAssets();

	_rootNode = Database::GetRootAssetTreeNode();
	_rootNode->isExpanded = true;

	// Init Editor Context
	editorContext = new EditorContext();
	editorRendererDomain = new EditorRendererDomain();
	editorRendererDomain->Inject(editorContext);
	editorRendererDomain->Init();

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
		// - UI Layout
		ImVec2 projectLeftPanelMin = EDITOR_WINDOW_PROJECT_POSITION;
		ImVec2 projectLeftPanelMax = projectLeftPanelMin;
		projectLeftPanelMax.x += EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
		projectLeftPanelMax.y += EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT;
		ImVec2 projectRightPanelMin = projectLeftPanelMax;
		projectRightPanelMin.y -= EDITOR_WINDOW_PROJECT_HEIGHT;
		ImVec2 projectRightPanelMax = projectRightPanelMin;
		projectRightPanelMax.x += EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH;
		projectRightPanelMax.y += EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT;

		// Device Input
		TickInput();
		if (GetMouseButtonDown(MouseButtons_Left)) {
			vec2 mousePos = GetMousePos();
			if (_IsInAABB(mousePos, projectLeftPanelMin, projectLeftPanelMax)) {
				_curChoosedPanelFlags = EditorPanelFlags_ProjectLeftColunm;
			}
			else if (_IsInAABB(mousePos, projectRightPanelMin, projectRightPanelMax)) {
				_curChoosedPanelFlags = EditorPanelFlags_ProjectRightColunm;
			}
			std::cout << "Choosed Panel: " << _curChoosedPanelFlags << std::endl;
		}
		if (GetKeyDown(ImGuiKey_Backspace)) {
			if (_curProjectChoosedNode != nullptr && _curProjectChoosedNode->fatherNode != nullptr) {
				_curProjectChoosedNode = _curProjectChoosedNode->fatherNode;
				_curProjectDetailsChoosedNode = nullptr;
			}
		}
		glfwPollEvents();

		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// - Project Panel
		ImGui::SetNextWindowPos(EDITOR_WINDOW_PROJECT_POSITION);
		ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_PROJECT_WIDTH, EDITOR_WINDOW_PROJECT_HEIGHT));
		ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH);
		ImGui::SetColumnWidth(1, EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH);
		ImGui_ShowProjectPanel();

		ImGui::NextColumn();

		if (_curProjectChoosedNode != nullptr) {
			ImGui_ShowProjectDetailsPanel(_curProjectChoosedNode);
		}
		ImGui::End();

		// - Editor Panel
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(100, 100));
		ImGui::Begin("Edit", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button("ClearInvalid")) {
			Database::ClearMetaFile();
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