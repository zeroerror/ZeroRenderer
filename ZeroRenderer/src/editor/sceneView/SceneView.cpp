#include "SceneView.h"

SceneView::SceneView(Camera* camera) {
	this->_fsmCom = new SceneViewFSMCom();
	this->cameraDistance = 10.0f;
	this->_sceneViewCamera = camera;
}

SceneView::~SceneView() {
	if (this->_fsmCom != nullptr) delete this->_fsmCom;
}

SceneViewFSMCom* SceneView::FSMCom() { return this->_fsmCom; }
Camera* SceneView::SceneViewCamera() { return this->_sceneViewCamera; }