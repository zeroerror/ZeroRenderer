#include "SceneView.h"

SceneView::SceneView() {
	this->_fsmCom = new SceneViewFSMCom();
}

SceneView::~SceneView() {
	if (this->_fsmCom != nullptr) delete this->_fsmCom;
}

SceneViewFSMCom* SceneView::FSMCom() {
	return this->_fsmCom;
}