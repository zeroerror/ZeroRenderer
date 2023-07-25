#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image/stb_image.h>

#include "EditorDatabase.h"
#include "Serialization.h"
#include <RuntimeDomain.h>
#include <RuntimeDomain.h>
#include "FileSuffix.h"

using namespace Serialization;
using namespace glm;
using namespace std;

enum EditorPanelFlags_ {
	EditorPanelFlags_None = 0,
	EditorPanelFlags_TitleBar,
	EditorPanelFlags_SceneView,
	EditorPanelFlags_ProjectLeftColunm,
	EditorPanelFlags_ProjectRightColunm,
};

enum KeyStatus_ {
	KeyStatus_None = 0,
	KeyStatus_Up = 1,
	KeyStatus_Down = 2,
	KeyStatus_Pressing = 3,
};

enum MouseButtons_ {
	MouseButtons_None = -1,
	MouseButtons_Left = 0,
	MouseButtons_Right = 1,
	MouseButtons_Middle = 2,
};

class EditorApp {

public:
	RuntimeContext* _runtimeContext;
	RuntimeDomain* _runtimeDomain;
	GLFWwindow* window;

	EditorApp();
	~EditorApp();
	int Tick();

private:
	bool _alreadyShutDown;
	void _ShutDown();

#pragma region [EDITOR WINDOW Config]

private:
	int EDITOR_WINDOW_WIDTH;
	int EDITOR_WINDOW_HEIGHT;
	ImVec2 EDITOR_WINDOW_TITLE_BAR_POS;
	int EDITOR_WINDOW_TITLE_BAR_WIDTH;
	int EDITOR_WINDOW_TITLE_BAR_HEIGHT;
	ImVec2 EDITOR_WINDOW_TITLE_BAR_POS_MIN;
	ImVec2 EDITOR_WINDOW_TITLE_BAR_POS_MAX;

	ImVec2 EDITOR_WINDOW_SCENE_POS;
	int EDITOR_WINDOW_SCENE_WIDTH;
	int EDITOR_WINDOW_SCENE_HEIGHT;
	ImVec2 EDITOR_WINDOW_SCENE_POS_MIN;
	ImVec2 EDITOR_WINDOW_SCENE_POS_MAX;

	ImVec2 EDITOR_WINDOW_PROJECT_POS;
	int EDITOR_WINDOW_PROJECT_WIDTH;
	int EDITOR_WINDOW_PROJECT_HEIGHT;

	int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
	int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT;
	ImVec2 EDITOR_WINDOW_PROJECT_LEFT_POS_MIN;
	ImVec2 EDITOR_WINDOW_PROJECT_LEFT_POS_MAX;

	int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH;
	int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT;
	ImVec2 EDITOR_WINDOW_PROJECT_RIGHT_POS_MIN;
	ImVec2 EDITOR_WINDOW_PROJECT_RIGHT_POS_MAX;
	float EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED;
	float EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED;

	void _InitWindowCongfig();

#pragma endregion

#pragma region [EDITOR TIME]

private:
	float _lastTime;
	float _deltaTime;

	void _TickDeltaTime();

#pragma endregion

#pragma region [EDITOR CACHE]

private:
	AssetTreeNode* _rootNode;
	AssetTreeNode* _curProjectChoosedNode;
	AssetTreeNode* _curProjectDetailsChoosedNode;
	EditorPanelFlags_ _curChoosedPanelFlags;
	double _assetClickTime;
	unsigned int _texture_id;
	void _InitEditorGLIcon();

#pragma endregion

#pragma region [Device Input]

public:
	vec2 GetMousePosDelta();
	vec2 GetMousePos();
	bool GetMouseButtonDown(const MouseButtons_& button);
	bool GetMouseButtonPressing(const MouseButtons_& button);
	bool GetMouseButtonUp(const MouseButtons_& button);
	bool GetKeyDown(const ImGuiKey& key);
	bool GetKeyPressing(const ImGuiKey& key);
	bool GetKeyUp(const ImGuiKey& key);

private:
	KeyStatus_ _allKeyStatus[1024];
	vec2 _lastMousePos;
	vec2 _mousePosDelta;

	bool _CheckMouseDown(const MouseButtons_& button);
	bool _CheckKeyDown(const ImGuiKey& key);
	bool _IsInAABB(const vec2& v, const ImVec2& min, const ImVec2& max);
	int _Clamp(const int& v, const int& min, const int& max);

	void _TickMouseKeyStatus(const MouseButtons_& key);
	void _TickKeyStatus(const ImGuiKey& key);
	void _TickEditorInput();

#pragma endregion

#pragma region [Project Panel]

private:
	void _ShowProjectPanel(AssetTreeNode* node, string dir, float xOffset);
	void _ShowProjectPanel();
	void _ShowProjectDetailsPanel(const AssetTreeNode* node);

#pragma endregion

#pragma region [Scene View]

private:
	GLuint _frameBuffer;
	GLuint _sceneViewTexture;
	void _InitSceneViewFrameBuffer();
	void _RenderSceneViewFrameBuffer();

#pragma endregion

#pragma region [EDITOR EVENT]

private:
	void _Event_PanelSelection();
	void _Event_DirectoryBackward();
	void _Event_SceneView(const float& deltaTime);
	void _TickEvents();

#pragma endregion

#pragma region [GL]

private:
	void _ShowFPS(GLFWwindow* window);
	void _ImGui_NewFrame();
	void _ShowEditorProjectPanle();
	void _ShowEditorTitleBar();
	void _ShowEditorSceneView();
	void _ImGuiShutDown();
	void _GLShutDown();

#pragma endregion

};