#pragma once

#include "SceneViewFSMState.h"
#include "SceneViewFSMFocusingModel.h"

class SceneViewFSMCom
{

public:
	SceneViewFSMCom();
	~SceneViewFSMCom();

    SceneViewFSMState_ FSMState();
    
    SceneViewFSMFocusingModel* FocusingModel();
    void Enter_Focusing(const vec3& startPos, const vec3& endPos, const float& duration);
    void Enter_None();

private:
    SceneViewFSMState_ _fsmState;
    SceneViewFSMFocusingModel* _focusingModel;

};

