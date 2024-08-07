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

enum EditorPanelFlags_
{
	EditorPanelFlags_None = 0,
	EditorPanelFlags_TitleBar,
	EditorPanelFlags_SceneView,
	EditorPanelFlags_Hierarchy,
	EditorPanelFlags_Inspector,
	EditorPanelFlags_ProjectLeftColunm,
	EditorPanelFlags_ProjectRightColunm,
};

class EditorApp
{

public:
	RuntimeContext *_runtimeContext;
	RuntimeDomain *_runtimeDomain;
	EditorContext *_editorContext;
	EditorDomain *_editorDomain;
	GLFWwindow *window;

	EditorApp();
	~EditorApp();
	int Tick();

private:
	bool _alreadyShutDown;
	void _ShutDown();

#pragma region[Editor Time]

private:
	float _lastTime;
	float _deltaTime;
	void _TickDeltaTime();

#pragma endregion

#pragma region[Editor Cahe]

private:
	EditorPanelFlags_ _curChoosedPanelFlags;
	void _InitEditorGLIcon();

#pragma endregion

#pragma region[Title Bar]

private:
	void _ShowTitleBarCanvas();

#pragma endregion

#pragma region[Hierarchy]

private:
	GameObject *_curHierarchyChoosedGameObject;
	unordered_map<GameObject *, bool> _hierarchyGameObjectFoldExpandMap;
	vector<GameObject *> _hierarchyRootGameObjects;
	double _hierarchyGameObjectClickTime;

	/**
	 * @brief Init the hierarchy of the scene
	 */
	void _InitHierarchy();

	/**
	 * @brief Show the hierarchy canvas
	 */
	void _ShowHierarchyCanvas();

	/**
	 * @brief Show the hierarchy of the scene
	 */
	void _ShowHierarchy(const Transform *transform, int depth);

	/**
	 * @brief Check if the game object is shown in the hierarchy
	 */
	bool _IsHierarchyGameObjectShow(const GameObject *gameObject);

#pragma endregion

#pragma region[Inspector]

private:
	/**
	 * @brief Show game object's info in the inspector
	 */
	void _ShowInspector(GameObject *go);

	/**
	 * @brief Show component's info in the inspector
	 */
	void _ShowInspectorComponent(const Component *component);

#pragma endregion

#pragma region[GUI]

	/**
	 * @brief Show GUI 'vec3' widget
	 */
	void GUI_ShowWidgetVec3(vec3 &vec, const char *format, const int padding, function<void(const vec3 &)> editCb);

	/**
	 * @brief Show GUI 'float' widget
	 */
	void GUI_ShowWidgetFloat(float &value, const char *format, const int padding, function<void(const float &)> editCb);

#pragma endregion

#pragma region[Project]

private:
	AssetTreeNode *_projectRootNode;
	AssetTreeNode *_curProjectChoosedNode;
	AssetTreeNode *_curProjectDetailsChoosedNode;
	unsigned int _projectFolderTextureID;
	double _projectAssetClickTime;

	void _ShowProjectLeftColumnCanvas();
	void _ShowProjectRightColumnCanvas();
	void _ShowProjectMainPanel(AssetTreeNode *node, string dir, float xOffset);
	void _ShowProjectDetailsPanel(const AssetTreeNode *node);

#pragma endregion

#pragma region[Scene View]

private:
	GLuint _frameBuffer;
	GLuint _sceneViewTexture;
	GLuint _depthTexture;
	void _InitSceneView();
	void _InitSceneViewFrameBuffer();
	void _RenderSceneViewFrameBuffer();
	void _ShowSceneViewCanvas(const vec2 &min, const vec2 &max);
	void _TickSceneView(const float &dt);

#pragma endregion

#pragma region[Editor Event]

private:
	void _Tick_Event_DirectoryBackward();
	void _Tick_Event_SceneView(const float &deltaTime);
	void _Tick_Events();

#pragma endregion

#pragma region[GL]

private:
	void _ShowFPS(GLFWwindow *window);
	void _ImGui_NewFrame();
	void _ImGuiShutDown();
	void _GLShutDown();

#pragma endregion

#pragma region[Editor Input]

	void _Tick_EditorInput();

#pragma endregion

#pragma region[Editor Window Config]

private:
	int EDITOR_WINDOW_WIDTH;
	int EDITOR_WINDOW_HEIGHT;

	int EDITOR_WINDOW_TITLE_BAR_WIDTH;
	int EDITOR_WINDOW_TITLE_BAR_HEIGHT;

	int EDITOR_WINDOW_SCENE_WIDTH;
	int EDITOR_WINDOW_SCENE_HEIGHT;

	int EDITOR_WINDOW_HIERARCHY_WIDTH;
	int EDITOR_WINDOW_HIERARCHY_HEIGHT;

	int EDITOR_WINDOW_INSPECTOR_WIDTH;
	int EDITOR_WINDOW_INSPECTOR_HEIGHT;

	int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_WIDTH;
	int EDITOR_WINDOW_PROJECT_LEFT_COLUNM_HEIGHT;

	int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_WIDTH;
	int EDITOR_WINDOW_PROJECT_RIGHT_COLUNM_HEIGHT;

	float EDITOR_WINDOW_SCENE_VIEW_ROTATE_SPEED;
	float EDITOR_WINDOW_SCENE_VIEW_MOVE_SPEED;

	void _InitEditorWindowCongfig();

#pragma endregion
};