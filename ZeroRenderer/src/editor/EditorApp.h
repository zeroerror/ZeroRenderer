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
#include <EditorDomain.h>
#include "FileSuffix.h"

#include "EditorUICanvas.h"

using namespace Serialization;
using namespace glm;
using namespace std;

enum EditorPanelFlags_ {
	EditorPanelFlags_None = 0,
	EditorPanelFlags_TitleBar,
	EditorPanelFlags_SceneView,
	EditorPanelFlags_Hierarchy,
	EditorPanelFlags_ProjectLeftColunm,
	EditorPanelFlags_ProjectRightColunm,
};

class EditorApp {

public:
	RuntimeContext* _runtimeContext;
	RuntimeDomain* _runtimeDomain;
	EditorContext* _editorContext;
	EditorDomain* _editorDomain;
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

	int EDITOR_WINDOW_TITLE_BAR_WIDTH;
	int EDITOR_WINDOW_TITLE_BAR_HEIGHT;

	int EDITOR_WINDOW_SCENE_WIDTH;
	int EDITOR_WINDOW_SCENE_HEIGHT;

	int EDITOR_WINDOW_HIERARCHY_WIDTH;
	int EDITOR_WINDOW_HIERARCHY_HEIGHT;

	int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
	int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT;

	int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH;
	int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT;

	float EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED;
	float EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED;

	void _InitEditorWindowCongfig();

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

#pragma region [Editor Canvas]

private:
	void _ShowTitleBarCanvas();
	void _ShowSceneViewCanvas(const vec2& min, const vec2& max);
	void _ShowProjectLeftColumnCanvas();
	void _ShowProjectRightColumnCanvas();
	void _ShowProjectMainPanel();
	void _ShowProjectMainPanel(AssetTreeNode* node, string dir, float xOffset);
	void _ShowProjectDetailsPanel(const AssetTreeNode* node);

#pragma endregion

#pragma region [Scene View]

private:
	GLuint _frameBuffer;
	GLuint _sceneViewTexture;
	void _InitSceneView();
	void _InitSceneViewFrameBuffer();
	void _RenderSceneViewFrameBuffer();

#pragma endregion

#pragma region [EDITOR EVENT]

private:
	void _Tick_Event_DirectoryBackward();
	void _Tick_Event_SceneView(const float& deltaTime);
	void _Tick_Events();

#pragma endregion

#pragma region [GL]

private:
	void _ShowFPS(GLFWwindow* window);
	void _ImGui_NewFrame();
	void _ImGuiShutDown();
	void _GLShutDown();

#pragma endregion

#pragma region [EDITOR INPUT]

	void _Tick_EditorInput();

#pragma endregion

};