#pragma once

#include "Camera.h"
#include "SceneViewFSMCom.h"

class SceneView{
    
public:
    SceneView();
    ~SceneView();

	Camera* camera;
    SceneViewFSMCom* FSMCom();

private:
    SceneViewFSMCom* _fsmCom;

};

