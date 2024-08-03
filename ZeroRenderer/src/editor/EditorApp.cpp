#include "EditorApp.h"
#include "AlignType.h"
#include "EditorUICanvas.h"
#include "EditorInputManager.h"

// ********************** EDITOR USER CONFIG **********************
EditorApp::EditorApp()
{

	// Init Editor Config
	_InitEditorWindowCongfig();

	// Init Editor
	_InitEditorGLIcon();

	// Init GL
	glfwInit();
	window = glfwCreateWindow(EDITOR_WINDOW_WIDTH, EDITOR_WINDOW_HEIGHT, "Zero Engine v0.0.1", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	// Init ImGui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Import Editor Database
	EditorDatabase::ImportAssets();
	_projectRootNode = EditorDatabase::GetRootAssetTreeNode();
	_projectRootNode->isExpanded = true;

	// Init Editor's Runtime Context And Domain
	_runtimeContext = new RuntimeContext();
	_runtimeDomain = new RuntimeDomain();
	_runtimeDomain->Inject(_runtimeContext);
	_runtimeDomain->PreprocessModelMeshes();

	// Init Editor Context And Domain
	_editorContext = new EditorContext();
	_editorDomain = new EditorDomain();
	_editorDomain->Inject(_editorContext, _runtimeDomain);

	// Init Scene View
	_InitSceneView();
	_InitSceneViewFrameBuffer();

	// Init Hierarchy
	_InitHierarchy();

	// Editor Canvas
	_editorContext->rootCanvas = new EditorUICanvas(Rect(1920, 1080, 0, 0, AnchorPointType::LeftTop), AlignType::LeftTop);

	_editorContext->titleBarCanvasNode = new EditorUICanvasNode(Rect(EDITOR_WINDOW_TITLE_BAR_WIDTH, EDITOR_WINDOW_TITLE_BAR_HEIGHT, 0, 0, AnchorPointType::LeftTop), AlignType::LeftTop);
	_editorContext->titleBarCanvasNode->callback = [this](const Rect &rect)
	{
		_ShowTitleBarCanvas();
	};

	_editorContext->sceneViewCanvasNode = new EditorUICanvasNode(Rect(EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT, 0, 0, AnchorPointType::LeftTop), AlignType::LeftTop);
	_editorContext->sceneViewCanvasNode->callback = [this](const Rect &rect)
	{
		_ShowSceneViewCanvas(rect.GetLeftTop(), rect.GetRightBottom());
	};

	_editorContext->hierarchyCanvasNode = new EditorUICanvasNode(Rect(EDITOR_WINDOW_HIERARCHY_WIDTH, EDITOR_WINDOW_HIERARCHY_HEIGHT, 0, 0, AnchorPointType::LeftTop), AlignType::LeftTop);
	_editorContext->hierarchyCanvasNode->callback = [this](const Rect &rect)
	{
		_ShowHierarchyCanvas();
	};

	_editorContext->projectLeftCanvasNode = new EditorUICanvasNode(Rect(EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH, EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT, 0, 0, AnchorPointType::LeftTop), AlignType::LeftTop);
	_editorContext->projectLeftCanvasNode->callback = [this](const Rect &rect)
	{
		_ShowProjectLeftColumnCanvas();
	};

	_editorContext->projectRightCanvasNode = new EditorUICanvasNode(Rect(EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH, EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT, 0, 0, AnchorPointType::LeftTop), AlignType::LeftTop);
	_editorContext->projectRightCanvasNode->callback = [this](const Rect &rect)
	{
		_ShowProjectRightColumnCanvas();
	};

	_editorContext->titleBarCanvasNode->name = "TitleBarCanvasNode";
	_editorContext->sceneViewCanvasNode->name = "SceneViewCanvasNode";
	_editorContext->hierarchyCanvasNode->name = "HierarchyCanvasNode";
	_editorContext->projectLeftCanvasNode->name = "ProjectLeftCanvasNode";
	_editorContext->projectRightCanvasNode->name = "ProjectRightCanvasNode";

	_editorContext->rootCanvas->Add(_editorContext->titleBarCanvasNode);
	_editorContext->rootCanvas->Add(_editorContext->sceneViewCanvasNode);
	_editorContext->rootCanvas->Add(_editorContext->hierarchyCanvasNode);
	_editorContext->rootCanvas->Add(_editorContext->projectLeftCanvasNode);
	_editorContext->rootCanvas->Add(_editorContext->projectRightCanvasNode);
}

EditorApp::~EditorApp()
{
	_ShutDown();
}

int EditorApp::Tick()
{
	if (glfwWindowShouldClose(window))
	{
		_ShutDown();
		return 0;
	}

	// ======== Input ========
	_Tick_EditorInput();

	// ======== Logic ========
	_TickDeltaTime();
	_Tick_Events();
	_TickSceneView(_deltaTime);

	// ======== Renderer ========
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_RenderSceneViewFrameBuffer();
	_ShowFPS(window);

	_ImGui_NewFrame();
	_editorContext->rootCanvas->Draw();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
	glfwPollEvents();

	return 1;
}

void EditorApp::_ShutDown()
{
	if (_alreadyShutDown)
		return;

	_ImGuiShutDown();
	_GLShutDown();
	_alreadyShutDown = true;
}

GLuint _frameBuffer;
GLuint _sceneViewTexture;
GLuint _depthTexture;
void EditorApp::_InitSceneViewFrameBuffer()
{
	// 初始化GL默认参数
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	// 生成帧缓冲FBO
	glGenFramebuffers(1, &_frameBuffer);
	// 绑定帧缓冲, 开始初始化
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glGenTextures(1, &_sceneViewTexture);
	glBindTexture(GL_TEXTURE_2D, _sceneViewTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _sceneViewTexture, 0);
	// 初始化深度纹理
	glGenTextures(1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, EDITOR_WINDOW_SCENE_WIDTH, EDITOR_WINDOW_SCENE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);
	// 解除绑定, 保证不会影响后续操作
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EditorApp::_InitSceneView()
{
	SceneMeta *currentSceneMeta = new SceneMeta();
	_runtimeDomain->OpenScene("asset/DefaultScene.scene", *currentSceneMeta);
	_editorContext->sceneView = new SceneView(_runtimeContext->mainCamera);
	_editorContext->currentSceneMeta = currentSceneMeta;
}

void EditorApp::_RenderSceneViewFrameBuffer()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		Camera sceneCam = *_editorContext->sceneView->SceneViewCamera();
		// {
		// 	// 渲染深度图
		// 	_runtimeDomain->RendererSceneShadowMap(*_runtimeContext->currentScene, sceneCam);
		// }

		{
			// 渲染场景
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
			glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			_runtimeDomain->RenderScene(*_runtimeContext->currentScene, sceneCam);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// {
		// 	// TEST 查看深度图
		// 	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		// 	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 	glEnable(GL_DEPTH_TEST);
		// 	glDepthFunc(GL_LESS);
		// 	Camera dlCam = Camera();
		// 	dlCam.CopyFrom(sceneCam);
		// 	dlCam.transform = new Transform();
		// 	DirectLight *dl = _runtimeContext->sceneDirectLight;
		// 	dlCam.transform->SetPosition(dl->transform->GetPosition());
		// 	dlCam.transform->SetRotation(dl->transform->GetRotation());
		// 	_runtimeDomain->RenderScene(*_runtimeContext->currentScene, dlCam);
		// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// }
	}
}

void EditorApp::_InitEditorWindowCongfig()
{
	EDITOR_WINDOW_WIDTH = 1920;
	EDITOR_WINDOW_HEIGHT = 1080;

	EDITOR_WINDOW_TITLE_BAR_WIDTH = EDITOR_WINDOW_WIDTH;
	EDITOR_WINDOW_TITLE_BAR_HEIGHT = EDITOR_WINDOW_HEIGHT * 0.1f;

	EDITOR_WINDOW_SCENE_WIDTH = EDITOR_WINDOW_WIDTH * 0.75f;
	EDITOR_WINDOW_SCENE_HEIGHT = EDITOR_WINDOW_HEIGHT * 0.6f;

	EDITOR_WINDOW_HIERARCHY_WIDTH = EDITOR_WINDOW_WIDTH * 0.25f;
	EDITOR_WINDOW_HIERARCHY_HEIGHT = EDITOR_WINDOW_HEIGHT * 0.6f;

	EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH = EDITOR_WINDOW_WIDTH * 0.25f;
	EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT = EDITOR_WINDOW_HEIGHT * 0.3f;

	EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH = EDITOR_WINDOW_WIDTH - EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
	EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT = EDITOR_WINDOW_HEIGHT * 0.3f;

	EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED = 1.0f;
	EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED = 10.0f;
}

void EditorApp::_TickDeltaTime()
{
	float curTime = glfwGetTime();
	_deltaTime = curTime - _lastTime;
	_lastTime = curTime;
}

AssetTreeNode *_projectRootNode = nullptr;
AssetTreeNode *_curProjectChoosedNode = nullptr;
AssetTreeNode *_curProjectDetailsChoosedNode = nullptr;
double _projectAssetClickTime;
unsigned int _projectFolderTextureID;

void EditorApp::_InitEditorGLIcon()
{
	string path = "asset/texture/folder.png";
	int width, height, channels;
	unsigned char *imgData = stbi_load(path.c_str(), &width, &height, &channels, 4);
	glGenTextures(1, &_projectFolderTextureID);
	glBindTexture(GL_TEXTURE_2D, _projectFolderTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
}

#pragma endregion

#pragma region[Title Bar]

void EditorApp::_ShowTitleBarCanvas()
{
	ImGui::Begin("Edit", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	if (ImGui::Button("Clear All Meta Files"))
	{
		vector<string> suffixes = vector<string>();
		suffixes.push_back(FileSuffix::SUFFIX_META);
		unsigned int suffixFlag = FileSuffix::ToFileSuffixFlag(suffixes);
		EditorDatabase::ClearFile(suffixFlag);
	}
	ImGui::SameLine();
	if (ImGui::Button("Generate Default Scene"))
	{
		EditorDatabase::GenerateDefaultSceneMeta();
	}
	ImGui::SameLine();
	if (ImGui::Button("Generate Default Shader"))
	{
		EditorDatabase::GenerateDefaultShader();
	}
	ImGui::End();
}

#pragma endregion

#pragma region[Scene View]

void EditorApp::_ShowSceneViewCanvas(const vec2 &min, const vec2 &max)
{
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImDrawList *drawList = ImGui::GetWindowDrawList();
	drawList->PushTextureID((ImTextureID)(uintptr_t)_sceneViewTexture);
	drawList->AddImage((ImTextureID)(uintptr_t)_sceneViewTexture, ImVec2(min.x, min.y), ImVec2(max.x, max.y), ImVec2(0, 1), ImVec2(1, 0));
	drawList->PopTextureID();
	ImGui::End();
}

void EditorApp::_TickSceneView(const float &dt)
{
	SceneView *sceneView = _editorContext->sceneView;
	if (sceneView == nullptr)
		return;

	auto fsmCom = sceneView->FSMCom();
	auto fsmState = fsmCom->FSMState();
	if (fsmState == SceneViewFSMState_None)
		return;

	if (fsmState == SceneViewFSMState_Focusing)
	{
		auto focusingModel = fsmCom->FocusingModel();
		if (focusingModel->isEntering)
		{
			focusingModel->isEntering = false;
		}

		focusingModel->Tick(dt);

		vec3 curCamPos = focusingModel->GetCurrentPos();
		sceneView->SceneViewCamera()->transform->SetPosition(curCamPos);

		if (focusingModel->IsExiting())
		{
			fsmCom->Enter_None();
		}
	}
}

#pragma endregion

#pragma region[Hierarchy]

void EditorApp::_InitHierarchy()
{
	_hierarchyGameObjectFoldExpandMap = unordered_map<GameObject *, bool>();
	_hierarchyRootGameObjects = vector<GameObject *>();

	vector<GameObject *> *gameObjects = _runtimeContext->currentScene->gameObjects;
	for (GameObject *go : *gameObjects)
	{
		_hierarchyGameObjectFoldExpandMap.insert(make_pair(go, false));
		if (go->transform()->GetFather() == nullptr)
		{
			_hierarchyRootGameObjects.push_back(go);
		}
	}
}

bool EditorApp::_IsHierarchyGameObjectShow(const GameObject *go)
{
	if (go == nullptr)
		return false;
	Transform *father = go->transform()->GetFather();
	if (father == nullptr)
		return true;

	return _hierarchyGameObjectFoldExpandMap[father->gameObject];
}

void EditorApp::_ShowHierarchyCanvas()
{
	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	for (GameObject *go : _hierarchyRootGameObjects)
	{
		Transform *curTrans = go->transform();
		_ShowHierarchy(curTrans, 0);
	}

	ImGui::End();
}

void EditorApp::_ShowHierarchy(const Transform *tran, int depth)
{
	if (tran == nullptr)
		return;

	GameObject *go = tran->gameObject;

	ImGui::Indent(depth * 20.0f);
	if (ImGui::Selectable(go->GetName().c_str(), _curHierarchyChoosedGameObject == go))
	{
		double nowClickTime = glfwGetTime();
		double clickTimeOffset = nowClickTime - _hierarchyGameObjectClickTime;
		_hierarchyGameObjectClickTime = nowClickTime;

		if (clickTimeOffset < 0.2f)
		{
			_editorDomain->SceneView_FocusOn(go->transform()->GetPosition());
			if (go->transform()->GetChildCount() > 0)
			{
				_hierarchyGameObjectFoldExpandMap.at(go) = !_hierarchyGameObjectFoldExpandMap.at(go);
			}
		}

		_curHierarchyChoosedGameObject = go;
	}

	if (_hierarchyGameObjectFoldExpandMap.at(go))
	{
		int childCount = tran->GetChildCount();
		for (int i = 0; i < childCount; i++)
		{
			_ShowHierarchy(tran->GetChild(i), depth + 1);
		}
	}

	ImGui::Unindent(depth * 20.0f);
}

#pragma endregion

#pragma region[Project]

void EditorApp::_ShowProjectLeftColumnCanvas()
{
	ImGui::Begin("Project Left Column", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	float xoffset = 0;
	_ShowProjectMainPanel(_projectRootNode, "", xoffset);
	ImGui::End();
}

void EditorApp::_ShowProjectRightColumnCanvas()
{
	ImGui::Begin("Project Right Column", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	if (_curProjectChoosedNode != nullptr)
	{
		_ShowProjectDetailsPanel(_curProjectChoosedNode);
	}
	ImGui::End();
}

void EditorApp::_ShowProjectMainPanel(AssetTreeNode *node, string dir, float xOffset)
{
	ImGui::Indent(xOffset);
	if (node->isDir)
	{
		ImGui::Image(reinterpret_cast<ImTextureID>(_projectFolderTextureID), ImVec2(32, 32)); // 调整图标大小
		ImGui::SameLine();
	}
	string assetName = node->assetName;
	const char *assetNamec = assetName.c_str();
	string choosedPath = dir + assetName;
	ImGui::PushID(1);
	if (ImGui::Selectable(assetNamec, _curProjectChoosedNode == node))
	{
		double nowClickTime = glfwGetTime();
		double clickTimeOffset = nowClickTime - _projectAssetClickTime;
		_projectAssetClickTime = nowClickTime;

		if (node->isDir)
		{
			_curProjectChoosedNode = node;
			if (clickTimeOffset < 0.2f)
			{
				node->isExpanded = !node->isExpanded;
			}
		}
	}
	ImGui::PopID();

	if (!node->isExpanded)
	{
		ImGui::Unindent(xOffset);
		return;
	}

	// Child
	xOffset += 10.0f;
	choosedPath += "/";
	for (auto kvp : node->childNodes)
	{
		AssetTreeNode *childNode = kvp.second;
		_ShowProjectMainPanel(childNode, choosedPath, xOffset);
	}
}

void EditorApp::_ShowProjectDetailsPanel(const AssetTreeNode *node)
{
	ImGui::Text(_curProjectChoosedNode->assetPath.c_str());
	ImGui::Spacing();
	ImGui::Indent(10.0f);
	ImGui::PushID(2);
	for (auto kvp : node->childNodes)
	{
		AssetTreeNode *node = kvp.second;
		if (node->isDir)
		{
			ImGui::Image(reinterpret_cast<ImTextureID>(_projectFolderTextureID), ImVec2(32, 32));
			ImGui::SameLine();
		}
		const char *assetNamec = node->assetName.c_str();
		if (ImGui::Selectable(assetNamec, _curProjectDetailsChoosedNode == node))
		{
			_curProjectDetailsChoosedNode = node;
			double nowClickTime = glfwGetTime();
			double clickTimeOffset = nowClickTime - _projectAssetClickTime;
			_projectAssetClickTime = nowClickTime;
			if (clickTimeOffset < 0.2f)
			{
				AssetTreeNode *detailNode;
				if (_curProjectDetailsChoosedNode->isDir)
				{
					_curProjectChoosedNode = _curProjectDetailsChoosedNode;
					_curProjectDetailsChoosedNode->isExpanded = true;
				}
			}
		}
	}
	ImGui::PopID();
}

#pragma endregion

#pragma region[EDITOR EVENT]

void EditorApp::_Tick_Event_DirectoryBackward()
{
	if (EditorInputManager::GetKeyDown(ImGuiKey_Backspace))
	{
		if (_curProjectChoosedNode != nullptr && _curProjectChoosedNode->fatherNode != nullptr)
		{
			_curProjectChoosedNode = _curProjectChoosedNode->fatherNode;
			_curProjectDetailsChoosedNode = nullptr;
		}
	}
}

void EditorApp::_Tick_Event_SceneView(const float &deltaTime)
{
	if (_curChoosedPanelFlags != EditorPanelFlags_SceneView)
		return;

	auto trans = _editorContext->sceneView->SceneViewCamera()->transform;
	// auto trans = _runtimeContext->sceneDirectLight->transform;
	auto pos = trans->GetPosition();
	auto rot = trans->GetRotation();
	auto camForward = trans->GetForward();
	auto camRight = trans->GetRight();

	if (EditorInputManager::GetMouseButtonPressing(MouseButtons_Right))
	{
		vec2 mousePosDelta = EditorInputManager::GetMousePosDelta();
		float rotateFactor = EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED * deltaTime;
		auto yawRadius = -mousePosDelta.x * rotateFactor;
		auto pitchRadius = mousePosDelta.y * rotateFactor;
		rot = quat(vec3(0, yawRadius, 0)) * rot * quat(vec3(pitchRadius, 0, 0));
	}
	trans->SetRotation(rot);

	if (EditorInputManager::GetKeyPressing(ImGuiKey_W))
	{
		pos += camForward * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}

	if (EditorInputManager::GetKeyPressing(ImGuiKey_S))
	{
		pos += -camForward * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}

	if (EditorInputManager::GetKeyPressing(ImGuiKey_A))
	{
		pos += camRight * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}

	if (EditorInputManager::GetKeyPressing(ImGuiKey_D))
	{
		pos += -camRight * EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED * deltaTime;
	}
	trans->SetPosition(pos);
}

void EditorApp::_Tick_Events()
{
	EditorInputManager::_TickEditorInput();
	_Tick_Event_DirectoryBackward();
	_Tick_Event_SceneView(_deltaTime);
}

#pragma endregion

#pragma region[GL]

void EditorApp::_ImGui_NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorApp::_ShowFPS(GLFWwindow *window)
{
	int fps = (int)(1.0f / _deltaTime);
	glfwSetWindowTitle(window, ("Zero Engine v0.0.1 FPS: " + to_string(fps)).c_str());
}

void EditorApp::_ImGuiShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorApp::_GLShutDown()
{
	glfwTerminate();
}

#pragma endregion

#pragma region[EDITOR INPUT]

void EditorApp::_Tick_EditorInput()
{
	if (!EditorInputManager::GetMouseButtonDown(MouseButtons_Left))
	{
		return;
	}

	vec2 mousePos = EditorInputManager::GetMousePos();
	EditorUICanvasNode *hitCanvasNode;
	if (!_editorContext->rootCanvas->RaycastHitCanvas(mousePos, hitCanvasNode))
	{
		return;
	}

	if (hitCanvasNode == _editorContext->titleBarCanvasNode)
	{
		this->_curChoosedPanelFlags = EditorPanelFlags_TitleBar;
	}
	else if (hitCanvasNode == _editorContext->sceneViewCanvasNode)
	{
		this->_curChoosedPanelFlags = EditorPanelFlags_SceneView;
	}
	else if (hitCanvasNode == _editorContext->hierarchyCanvasNode)
	{
		this->_curChoosedPanelFlags = EditorPanelFlags_Hierarchy;
	}
	else if (hitCanvasNode == _editorContext->projectLeftCanvasNode)
	{
		this->_curChoosedPanelFlags = EditorPanelFlags_ProjectLeftColunm;
	}
	else if (hitCanvasNode == _editorContext->projectLeftCanvasNode)
	{
		this->_curChoosedPanelFlags = EditorPanelFlags_ProjectRightColunm;
	}
}

#pragma endregion
