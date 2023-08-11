#pragma once

#include "Camera.h"
#include "SceneViewFSMCom.h"

class SceneView {

public:
	SceneView(Camera* camera);
	~SceneView();

	SceneViewFSMCom* FSMCom();
	float cameraDistance;
	Camera* SceneViewCamera();

private:
	SceneViewFSMCom* _fsmCom;
	Camera* _sceneViewCamera;

};

