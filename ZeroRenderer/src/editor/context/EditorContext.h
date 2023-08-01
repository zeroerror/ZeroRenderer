#pragma once
#include "SceneMeta.h"
#include "Camera.h"

class EditorContext {

public:
	SceneMeta* currentSceneMeta;
	Camera* sceneViewCamera;

};