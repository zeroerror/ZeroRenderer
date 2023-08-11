#include "SceneViewFSMFocusingModel.h"

SceneViewFSMFocusingModel::SceneViewFSMFocusingModel() {}
SceneViewFSMFocusingModel::~SceneViewFSMFocusingModel() {}

void SceneViewFSMFocusingModel::Enter(const vec3& startPos, const vec3& endPos, const float& duration) {
	this->_focusingCameraStartPos = startPos;
	this->_focusingCameraEndPos = endPos;
	this->_focusingDuration = duration;
	this->_focusingTime = 0;
	this->isEntering = true;
}

void SceneViewFSMFocusingModel::Tick(const float& deltaTime) {
	this->_focusingTime += deltaTime;
}

vec3 SceneViewFSMFocusingModel::GetCurrentPos() const {
	float ratio = _focusingTime / _focusingDuration;
	vec3 offset = _focusingCameraEndPos - _focusingCameraStartPos;
	return _focusingCameraStartPos + ratio * offset;
}

bool SceneViewFSMFocusingModel::IsExiting() const {
	return this->_focusingTime >= this->_focusingDuration;
}
