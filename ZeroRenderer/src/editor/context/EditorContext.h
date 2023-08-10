#pragma once
#include "SceneMeta.h"
#include "Camera.h"
#include "EditorUICanvas.h"

class EditorContext {

public:
	EditorContext();
	~EditorContext();

	SceneMeta* currentSceneMeta;
	Camera* sceneViewCamera;

	EditorUICanvas* rootCanvas;
	EditorUICanvasNode* titleBarCanvasNode;
	EditorUICanvasNode* sceneViewCanvasNode;
	EditorUICanvasNode* hierarchyCanvasNode;
	EditorUICanvasNode* projectLeftCanvasNode;
	EditorUICanvasNode* projectRightCanvasNode;

};