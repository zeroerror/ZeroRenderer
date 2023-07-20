#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image/stb_image.h>

#include "EditorDatabase.h"
#include "Serialization.h"
#include <RuntimeDomain.h>
#include "FileSuffix.h"

using namespace Serialization;
using namespace glm;

// ********************** EDITOR Context **********************
RuntimeContext* runtimeContext;
RuntimeDomain* runtimeDomain;

// ********************** EDITOR USER CONFIG **********************
static const int EDITOR_WINDOW_WIDTH = 1920;
static const int EDITOR_WINDOW_HEIGHT = 1080;

static const ImVec2 EDITOR_WINDOW_TITLE_BAR_POS = ImVec2(0, 0);
static const int EDITOR_WINDOW_TITLE_BAR_WIDTH = EDITOR_WINDOW_WIDTH;
static const int EDITOR_WINDOW_TITLE_BAR_HEIGHT = 1.0f * EDITOR_WINDOW_HEIGHT / 10.0f;
static const ImVec2 EDITOR_WINDOW_TITLE_BAR_POS_MIN = EDITOR_WINDOW_TITLE_BAR_POS;
static const ImVec2 EDITOR_WINDOW_TITLE_BAR_POS_MAX = ImVec2(EDITOR_WINDOW_TITLE_BAR_POS_MIN.x + EDITOR_WINDOW_TITLE_BAR_WIDTH, EDITOR_WINDOW_TITLE_BAR_POS_MIN.y + EDITOR_WINDOW_TITLE_BAR_HEIGHT);

static const ImVec2 EDITOR_WINDOW_SCENE_POS = ImVec2(0, EDITOR_WINDOW_TITLE_BAR_HEIGHT);
static const int EDITOR_WINDOW_SCENE_WIDTH = EDITOR_WINDOW_WIDTH;
static const int EDITOR_WINDOW_SCENE_HEIGHT = 6.0f * EDITOR_WINDOW_HEIGHT / 10.0f;
static const ImVec2 EDITOR_WINDOW_SCENE_POS_MIN = EDITOR_WINDOW_SCENE_POS;
static const ImVec2 EDITOR_WINDOW_SCENE_POS_MAX = ImVec2(EDITOR_WINDOW_SCENE_POS_MIN.x + EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_POS_MIN.y + EDITOR_WINDOW_SCENE_HEIGHT);

static const ImVec2 EDITOR_WINDOW_PROJECT_POS = ImVec2(0, EDITOR_WINDOW_TITLE_BAR_HEIGHT + EDITOR_WINDOW_SCENE_HEIGHT);
static const int EDITOR_WINDOW_PROJECT_WIDTH = EDITOR_WINDOW_WIDTH;
static const int EDITOR_WINDOW_PROJECT_HEIGHT = 3.0f * EDITOR_WINDOW_HEIGHT / 10.0f;

static const int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH = EDITOR_WINDOW_PROJECT_WIDTH / 4.0f;
static const int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT = EDITOR_WINDOW_PROJECT_HEIGHT;
static const ImVec2 EDITOR_WINDOW_PROJECT_LEFT_POS_MIN = EDITOR_WINDOW_PROJECT_POS;
static const ImVec2 EDITOR_WINDOW_PROJECT_LEFT_POS_MAX = ImVec2(EDITOR_WINDOW_PROJECT_LEFT_POS_MIN.x + EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH, EDITOR_WINDOW_PROJECT_LEFT_POS_MIN.y + EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT);

static const int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH = EDITOR_WINDOW_WIDTH - EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
static const int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT = EDITOR_WINDOW_PROJECT_HEIGHT;
static const ImVec2 EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN = ImVec2(EDITOR_WINDOW_PROJECT_LEFT_POS_MAX.x, EDITOR_WINDOW_PROJECT_LEFT_POS_MAX.y - EDITOR_WINDOW_PROJECT_HEIGHT);
static const ImVec2 EDITOR_WINDOW_PROJECT_RIGHT_POS_MAX = ImVec2(EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN.x + EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH, EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN.y + EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT);

enum EditorPanelFlags_ {
	EditorPanelFlags_None = 0,
	EditorPanelFlags_TitleBar,
	EditorPanelFlags_SceneView,
	EditorPanelFlags_ProjectLeftColunm,
	EditorPanelFlags_ProjectRightColunm,
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

#pragma region [EDITOR EVENT]

void PanelSelection() {
	if (GetMouseButtonDown(MouseButtons_Left)) {
		vec2 mousePos = GetMousePos();
		if (_IsInAABB(mousePos, EDITOR_WINDOW_TITLE_BAR_POS_MIN, EDITOR_WINDOW_TITLE_BAR_POS_MAX)) {
			_curChoosedPanelFlags = EditorPanelFlags_TitleBar;
		}
		else if (_IsInAABB(mousePos, EDITOR_WINDOW_SCENE_POS_MIN, EDITOR_WINDOW_SCENE_POS_MAX)) {
			_curChoosedPanelFlags = EditorPanelFlags_SceneView;
		}
		else if (_IsInAABB(mousePos, EDITOR_WINDOW_PROJECT_LEFT_POS_MIN, EDITOR_WINDOW_PROJECT_LEFT_POS_MAX)) {
			_curChoosedPanelFlags = EditorPanelFlags_ProjectLeftColunm;
		}
		else if (_IsInAABB(mousePos, EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN, EDITOR_WINDOW_PROJECT_RIGHT_POS_MAX)) {
			_curChoosedPanelFlags = EditorPanelFlags_ProjectRightColunm;
		}
		std::cout << "Choosed Panel: " << _curChoosedPanelFlags << std::endl;
	}
}

void DirectoryBackward() {
	if (GetKeyDown(ImGuiKey_Backspace)) {
		if (_curProjectChoosedNode != nullptr && _curProjectChoosedNode->fatherNode != nullptr) {
			_curProjectChoosedNode = _curProjectChoosedNode->fatherNode;
			_curProjectDetailsChoosedNode = nullptr;
		}
	}
}

void TickEditorEvents() {
	TickInput();
	PanelSelection();
	DirectoryBackward();
}

#pragma endregion

void GL_CLEANUP() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

int main() {
	// vector<string> suffixes = vector<string>();
	// suffixes.push_back(FileSuffix::SUFFIX_MAT);
	// suffixes.push_back(FileSuffix::SUFFIX_META);
	// suffixes.push_back(FileSuffix::SUFFIX_SCENE);
	// unsigned int suffixFlag = FileSuffix::ToFileSuffixFlag(suffixes);
	// EditorDatabase::ClearFile(suffixFlag);

	// Import Editor Database
	EditorDatabase::ImportAssets();
	_rootNode = EditorDatabase::GetRootAssetTreeNode();
	_rootNode->isExpanded = true;

	// Init Editor Context
	runtimeContext = new RuntimeContext();
	runtimeDomain = new RuntimeDomain();
	runtimeDomain->Inject(runtimeContext);
	runtimeDomain->Init();

	// Init GL
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(EDITOR_WINDOW_WIDTH, EDITOR_WINDOW_HEIGHT, "Zero Engine v0.0.1", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	// Init ImGui
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

	// Init Scene's Frame Buffer
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	GLuint sceneViewTexture;
	glGenTextures(1, &sceneViewTexture);
	glBindTexture(GL_TEXTURE_2D, sceneViewTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneViewTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		TickEditorEvents();

		// - Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// - Editor Project Panel
		ImGui::SetNextWindowPos(EDITOR_WINDOW_PROJECT_POS);
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

		// - Editor Title Bar
		ImGui::SetNextWindowPos(EDITOR_WINDOW_TITLE_BAR_POS);
		ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_TITLE_BAR_WIDTH, EDITOR_WINDOW_TITLE_BAR_HEIGHT));
		ImGui::Begin("Edit", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button("Clear META")) {
			vector<string> suffixes = vector<string>();
			suffixes.push_back(FileSuffix::SUFFIX_META);
			unsigned int suffixFlag = FileSuffix::ToFileSuffixFlag(suffixes);
			EditorDatabase::ClearFile(suffixFlag);
		}
		ImGui::End();

		// - Editor Scene Panel

		// Render Scene's Frame Buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT);
		glBindTexture(GL_TEXTURE_2D, sceneViewTexture);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			glDrawBuffer(GL_COLOR_ATTACHMENT0);  // 指定绘制缓冲为颜色附件
			glReadBuffer(GL_NONE);  // 读取缓冲设置为GL_NONE
			runtimeDomain->RenderScene("asset/DefaultScene.scene");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Show Scene View By Frame Buffer Texture
		ImGui::SetNextWindowPos(EDITOR_WINDOW_SCENE_POS);
		ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT));
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 panelPos = ImGui::GetCursorScreenPos();
		ImVec2 panelPosMax = ImVec2(panelPos.x + EDITOR_WINDOW_SCENE_WIDTH, panelPos.y + EDITOR_WINDOW_SCENE_HEIGHT);
		drawList->PushTextureID((ImTextureID)(uintptr_t)sceneViewTexture);
		drawList->AddImage((ImTextureID)(uintptr_t)sceneViewTexture, panelPos, panelPosMax, ImVec2(0, 1), ImVec2(1, 0));
		drawList->PopTextureID();
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	GL_CLEANUP();

	return 0;
}