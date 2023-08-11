#include "SceneViewFSMCom.h"

SceneViewFSMCom::SceneViewFSMCom() {
    this->_focusingModel = new SceneViewFSMFocusingModel();
}

SceneViewFSMCom::~SceneViewFSMCom() {}

SceneViewFSMFocusingModel* SceneViewFSMCom::FocusingModel() {
    return this->_focusingModel;
}