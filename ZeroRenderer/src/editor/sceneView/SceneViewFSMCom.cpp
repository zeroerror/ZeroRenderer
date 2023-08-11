#include "SceneViewFSMCom.h"

#include <iostream>

using namespace std;

SceneViewFSMCom::SceneViewFSMCom() {
    this->_focusingModel = new SceneViewFSMFocusingModel();
    this->_fsmState = SceneViewFSMState_None;
}

SceneViewFSMCom::~SceneViewFSMCom() {}

SceneViewFSMState_ SceneViewFSMCom::FSMState() { return this->_fsmState; }

SceneViewFSMFocusingModel* SceneViewFSMCom::FocusingModel() { return this->_focusingModel; }

void SceneViewFSMCom::Enter_Focusing(const vec3& startPos, const vec3& endPos, const float& duration) {
    this->_focusingModel->Enter(startPos, endPos, duration);
    this->_fsmState = SceneViewFSMState_Focusing;
    cout << "SceneViewFSMCom Enter_Focusing " << endl;
}

void SceneViewFSMCom::Enter_None() {
    this->_fsmState = SceneViewFSMState_None;
    cout << "SceneViewFSMCom Enter_None " << endl;
}
