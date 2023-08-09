#pragma once
#include "SceneMeta.h"
#include "Camera.h"
#include "EditorUICanvas.h"

class EditorContext {

public:
	SceneMeta* currentSceneMeta;
	Camera* sceneViewCamera;

	EditorUICanvas* _rootCanvas;
	EditorUICanvasNode* _titleBarCanvasNode;
	EditorUICanvasNode* _sceneViewCanvasNode;
	EditorUICanvasNode* _projectCanvasNode;

};