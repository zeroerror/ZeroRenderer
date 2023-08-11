#pragma once

#include "SceneViewFSMFocusingModel.h"

class SceneViewFSMCom
{

public:
	SceneViewFSMCom();
	~SceneViewFSMCom();

    SceneViewFSMFocusingModel* FocusingModel();

private:
    SceneViewFSMFocusingModel* _focusingModel;

};

