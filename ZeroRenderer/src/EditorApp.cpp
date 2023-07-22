#include "EditorApp.h"

// ********************** EDITOR USER CONFIG **********************
EditorApp::EditorApp() {
	_InitWindowCongfig();

	// Import Editor Database
	EditorDatabase::ImportAssets();
	_rootNode = EditorDatabase::GetRootAssetTreeNode();
	_rootNode->isExpanded = true;

	// Init Editor Context And Domain
	_runtimeContext = new RuntimeContext();
	_runtimeDomain = new RuntimeDomain();
	_runtimeDomain->Inject(_runtimeContext);
	_runtimeDomain->Init();

	// Init GL
	glfwInit();
	window = glfwCreateWindow(EDITOR_WINDOW_WIDTH, EDITOR_WINDOW_HEIGHT, "Zero Engine v0.0.1", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	// Init ImGui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	_InitEditorGLIcon();

	// Init Scene's Frame Buffer
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

	glGenTextures(1, &_sceneViewTexture);
	glBindTexture(GL_TEXTURE_2D, _sceneViewTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _sceneViewTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_InitWindowCongfig();
}

EditorApp::~EditorApp() {
	ShutDown();
}

int EditorApp::Tick() {
	if (glfwWindowShouldClose(window)) {
		ShutDown();
		return 0;
	}


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	_TickDeltaTime();
	_TickEvents();
	_ShowFPS(window);

	_ImGui_NewFrame();
	_ImGui_ShowEditorProjectPanle();
	_ImGui_ShowEditorTitleBar();
	_ImGui_ShowEditorSceneView();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
	glfwPollEvents();

	return 1;
}

void EditorApp::ShutDown() {
	if (_alreadyShutDown)return;

	_ImGuiShutDown();
	_GLShutDown();
	_alreadyShutDown = true;
}


void EditorApp::_InitWindowCongfig() {
	EDITOR_WINDOW_WIDTH = 1920;
	EDITOR_WINDOW_HEIGHT = 1080;

	EDITOR_WINDOW_TITLE_BAR_POS = ImVec2(0, 0);
	EDITOR_WINDOW_TITLE_BAR_WIDTH = EDITOR_WINDOW_WIDTH;
	EDITOR_WINDOW_TITLE_BAR_HEIGHT = 1.0f * EDITOR_WINDOW_HEIGHT / 10.0f;
	EDITOR_WINDOW_TITLE_BAR_POS_MIN = EDITOR_WINDOW_TITLE_BAR_POS;
	EDITOR_WINDOW_TITLE_BAR_POS_MAX = ImVec2(EDITOR_WINDOW_TITLE_BAR_POS_MIN.x + EDITOR_WINDOW_TITLE_BAR_WIDTH, EDITOR_WINDOW_TITLE_BAR_POS_MIN.y + EDITOR_WINDOW_TITLE_BAR_HEIGHT);

	EDITOR_WINDOW_SCENE_POS = ImVec2(0, EDITOR_WINDOW_TITLE_BAR_HEIGHT);
	EDITOR_WINDOW_SCENE_WIDTH = EDITOR_WINDOW_WIDTH;
	EDITOR_WINDOW_SCENE_HEIGHT = 6.0f * EDITOR_WINDOW_HEIGHT / 10.0f;
	EDITOR_WINDOW_SCENE_POS_MIN = EDITOR_WINDOW_SCENE_POS;
	EDITOR_WINDOW_SCENE_POS_MAX = ImVec2(EDITOR_WINDOW_SCENE_POS_MIN.x + EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_POS_MIN.y + EDITOR_WINDOW_SCENE_HEIGHT);

	EDITOR_WINDOW_PROJECT_POS = ImVec2(0, EDITOR_WINDOW_TITLE_BAR_HEIGHT + EDITOR_WINDOW_SCENE_HEIGHT);
	EDITOR_WINDOW_PROJECT_WIDTH = EDITOR_WINDOW_WIDTH;
	EDITOR_WINDOW_PROJECT_HEIGHT = 3.0f * EDITOR_WINDOW_HEIGHT / 10.0f;

	EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH = EDITOR_WINDOW_PROJECT_WIDTH / 4.0f;
	EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT = EDITOR_WINDOW_PROJECT_HEIGHT;
	EDITOR_WINDOW_PROJECT_LEFT_POS_MIN = EDITOR_WINDOW_PROJECT_POS;
	EDITOR_WINDOW_PROJECT_LEFT_POS_MAX = ImVec2(EDITOR_WINDOW_PROJECT_LEFT_POS_MIN.x + EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH, EDITOR_WINDOW_PROJECT_LEFT_POS_MIN.y + EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT);

	EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH = EDITOR_WINDOW_WIDTH - EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
	EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT = EDITOR_WINDOW_PROJECT_HEIGHT;
	EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN = ImVec2(EDITOR_WINDOW_PROJECT_LEFT_POS_MAX.x, EDITOR_WINDOW_PROJECT_LEFT_POS_MAX.y - EDITOR_WINDOW_PROJECT_HEIGHT);
	EDITOR_WINDOW_PROJECT_RIGHT_POS_MAX = ImVec2(EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN.x + EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH, EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN.y + EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT);

	EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED = 1.0f;
	EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED = 10.0f;
}

void EditorApp::_TickDeltaTime() {
	float curTime = glfwGetTime();
	_deltaTime = curTime - _lastTime;
	_lastTime = curTime;
}

AssetTreeNode* _rootNode = nullptr;
AssetTreeNode* _curProjectChoosedNode = nullptr;
AssetTreeNode* _curProjectDetailsChoosedNode = nullptr;
EditorPanelFlags_ _curChoosedPanelFlags = EditorPanelFlags_None;
double _assetClickTime;
unsigned int _texture_id;

void EditorApp::_InitEditorGLIcon() {
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
}

#pragma endregion

#pragma region [Device Input]

KeyStatus_ _allKeyStatus[1024] = {};

bool EditorApp::EditorApp::_CheckMouseDown(const MouseButtons_& button) {
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseDown[button];
}

bool EditorApp::_CheckKeyDown(const ImGuiKey& key) {
	ImGuiIO& io = ImGui::GetIO();
	return io.KeysDown[key];
}

bool EditorApp::_IsInAABB(const vec2& v, const ImVec2& min, const ImVec2& max) {
	return v.x > min.x && v.x < max.x&& v.y>min.y && v.y < max.y;
}

int EditorApp::_Clamp(const int& v, const int& min, const int& max) {
	return v<min ? min : v>max ? max : v;
}

vec2 EditorApp::GetMousePosDelta() { return _mousePosDelta; }
vec2 EditorApp::GetMousePos() {
	ImVec2 p = ImGui::GetMousePos();
	return vec2(p.x, p.y);
}

bool EditorApp::GetMouseButtonDown(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Down; }
bool EditorApp::GetMouseButtonPressing(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Pressing; }
bool EditorApp::GetMouseButtonUp(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Up; }
bool EditorApp::GetKeyDown(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Down; }
bool EditorApp::GetKeyPressing(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Pressing; }
bool EditorApp::GetKeyUp(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Up; }

void EditorApp::_TickMouseKeyStatus(const MouseButtons_& key) {
	if (_CheckMouseDown(key))_allKeyStatus[key] = (KeyStatus_)_Clamp((_allKeyStatus[key] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[key] = (KeyStatus_)_Clamp((_allKeyStatus[key] - 1), KeyStatus_None, KeyStatus_Up);
}
void EditorApp::_TickKeyStatus(const ImGuiKey& key) {
	if (_CheckKeyDown(key))_allKeyStatus[key] = (KeyStatus_)_Clamp((_allKeyStatus[key] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[key] = (KeyStatus_)_Clamp((_allKeyStatus[key] - 1), KeyStatus_None, KeyStatus_Up);
}
void EditorApp::_TickEditorInput() {
	vec2 curMousePos = GetMousePos();
	_mousePosDelta = curMousePos - _lastMousePos;
	_lastMousePos = curMousePos;

	_TickMouseKeyStatus(MouseButtons_Left);
	_TickMouseKeyStatus(MouseButtons_Right);
	_TickMouseKeyStatus(MouseButtons_Middle);
	_TickKeyStatus(ImGuiKey_Backspace);
	_TickKeyStatus(ImGuiKey_W);
	_TickKeyStatus(ImGuiKey_S);
	_TickKeyStatus(ImGuiKey_A);
	_TickKeyStatus(ImGuiKey_D);
}

#pragma endregion

#pragma region [Project Panel]

void EditorApp::_ShowProjectPanel(AssetTreeNode* node, string dir, float xOffset) {
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
		_ShowProjectPanel(childNode, choosedPath, xOffset);
	}
}

void EditorApp::_ShowProjectPanel() {
	float xoffset = 0;
	_ShowProjectPanel(_rootNode, "", xoffset);
}

void EditorApp::_ShowProjectDetailsPanel(const AssetTreeNode* node) {
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

void EditorApp::_Event_PanelSelection() {
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

void EditorApp::_Event_DirectoryBackward() {
	if (GetKeyDown(ImGuiKey_Backspace)) {
		if (_curProjectChoosedNode != nullptr && _curProjectChoosedNode->fatherNode != nullptr) {
			_curProjectChoosedNode = _curProjectChoosedNode->fatherNode;
			_curProjectDetailsChoosedNode = nullptr;
		}
	}
}

void EditorApp::_Event_SceneView(const float& deltaTime) {
	if (_curChoosedPanelFlags != EditorPanelFlags_SceneView)return;

	auto mainCamera = _runtimeContext->mainCamera;
	auto pos = mainCamera->transform->GetPosition();
	auto rot = mainCamera->transform->rotation;
	auto camForward = mainCamera->transform->GetForward();
	auto camRight = mainCamera->transform->GetRight();

	if (GetMouseButtonPressing(MouseButtons_Right)) {
		// todo： to runtimedomain's sceneviewdomain
		vec2 mousePosDelta = GetMousePosDelta();
		float rotateFactor = EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED * deltaTime;
		auto yawRadius = -mousePosDelta.x * rotateFactor;
		auto pitchRadius = mousePosDelta.y * rotateFactor;
		rot = quat(vec3(0, yawRadius, 0)) * rot * quat(vec3(pitchRadius, 0, 0));
	}
	mainCamera->transform->SetRotation(rot);

	if (GetKeyPressing(ImGuiKey_W)) {
		pos += camForward * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}

	if (GetKeyPressing(ImGuiKey_S)) {
		pos += -camForward * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}

	if (GetKeyPressing(ImGuiKey_A)) {
		pos += camRight * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}

	if (GetKeyPressing(ImGuiKey_D)) {
		pos += -camRight * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}
	mainCamera->transform->SetPosition(pos);
}

void EditorApp::_TickEvents() {
	_TickEditorInput();
	_Event_PanelSelection();
	_Event_DirectoryBackward();
	_Event_SceneView(_deltaTime);
}

#pragma endregion

#pragma region [GL]

void EditorApp::_ShowFPS(GLFWwindow* window) {
	int fps = (int)(1.0f / _deltaTime);
	glfwSetWindowTitle(window, ("Zero Engine v0.0.1 FPS: " + to_string(fps)).c_str());
}

void EditorApp::_ImGui_NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorApp::_ImGui_ShowEditorProjectPanle() {
	ImGui::SetNextWindowPos(EDITOR_WINDOW_PROJECT_POS);
	ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_PROJECT_WIDTH, EDITOR_WINDOW_PROJECT_HEIGHT));
	ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH);
	ImGui::SetColumnWidth(1, EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH);
	_ShowProjectPanel();

	ImGui::NextColumn();

	if (_curProjectChoosedNode != nullptr) {
		_ShowProjectDetailsPanel(_curProjectChoosedNode);
	}
	ImGui::End();
}

void EditorApp::_ImGui_ShowEditorTitleBar() {
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
}

GLuint _frameBuffer;
GLuint _sceneViewTexture;
void EditorApp::_ImGui_ShowEditorSceneView() {
	// Render Scene's Frame Buffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		glViewport(0, 0, EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT);
		glBindTexture(GL_TEXTURE_2D, _sceneViewTexture);
		_runtimeDomain->RenderScene("asset/DefaultScene.scene");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Show Scene View By Frame Buffer Texture
	ImGui::SetNextWindowPos(EDITOR_WINDOW_SCENE_POS);
	ImGui::SetNextWindowSize(ImVec2(EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT));
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 panelPos = ImGui::GetCursorScreenPos();
	ImVec2 panelPosMax = ImVec2(panelPos.x + EDITOR_WINDOW_SCENE_WIDTH, panelPos.y + EDITOR_WINDOW_SCENE_HEIGHT);
	drawList->PushTextureID((ImTextureID)(uintptr_t)_sceneViewTexture);
	drawList->AddImage((ImTextureID)(uintptr_t)_sceneViewTexture, panelPos, panelPosMax, ImVec2(0, 1), ImVec2(1, 0));
	drawList->PopTextureID();
	ImGui::End();
}

void EditorApp::_ImGuiShutDown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorApp::_GLShutDown() {
	glfwTerminate();
}

#pragma endregion
