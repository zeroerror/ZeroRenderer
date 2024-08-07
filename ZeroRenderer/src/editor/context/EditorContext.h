#pragma once
#include "SceneMeta.h"
#include "Camera.h"
#include "EditorUICanvas.h"
#include "SceneView.h"

class EditorContext
{

public:
	EditorContext();
	~EditorContext();

	SceneMeta *currentSceneMeta;
	SceneView *sceneView;

	EditorUICanvas *rootCanvas;

	EditorUICanvasNode *titleBarCanvasNode;
	EditorUICanvasNode *sceneViewCanvasNode;
	EditorUICanvasNode *hierarchyCanvasNode;
	EditorUICanvasNode *inspectorCanvasNode;
	EditorUICanvasNode *projectLeftCanvasNode;
	EditorUICanvasNode *projectRightCanvasNode;
};