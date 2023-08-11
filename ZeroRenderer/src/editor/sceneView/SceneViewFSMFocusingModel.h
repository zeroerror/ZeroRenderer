#pragma once

#include "glm/glm.hpp"

using namespace glm;

class SceneViewFSMFocusingModel
{

public:
	SceneViewFSMFocusingModel();
	~SceneViewFSMFocusingModel();
	void Enter(const vec3& startPos, const vec3& endPos, const float& duration, const float& cameraDistance);
	void Tick(const float& deltaTime);

	vec3 GetCurrentPos() const;

private:
	bool _isEntering;

	vec3 _focusingCameraStartPos;
	vec3 _focusingCameraEndPos;
	float _focusingDuration;
	float _focusingTime;
	float _cameraDistance;
};