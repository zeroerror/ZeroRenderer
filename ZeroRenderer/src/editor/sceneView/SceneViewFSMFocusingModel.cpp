#include "SceneViewFSMFocusingModel.h"

SceneViewFSMFocusingModel::SceneViewFSMFocusingModel() {}
SceneViewFSMFocusingModel::~SceneViewFSMFocusingModel() {}

void SceneViewFSMFocusingModel::Enter(const vec3& startPos, const vec3& endPos, const float& duration, const float& cameraDistance) {
	this->_focusingCameraStartPos = startPos;
	this->_focusingCameraEndPos = endPos;
	this->_focusingDuration = duration;
	this->_cameraDistance = cameraDistance;
	this->_focusingTime = 0;
}

void SceneViewFSMFocusingModel::Tick(const float& deltaTime) {
	this->_focusingTime += deltaTime;
}

vec3 SceneViewFSMFocusingModel::GetCurrentPos() const {
	float ratio = this->_focusingTime / this->_focusingDuration;
	vec3 offset = this->_focusingCameraEndPos - this->_focusingCameraStartPos;
	return ratio * offset;
}
