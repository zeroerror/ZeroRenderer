#pragma once

#include "glm/glm.hpp"

using namespace glm;

class SceneViewFSMFocusingModel
{

public:
	SceneViewFSMFocusingModel();
	~SceneViewFSMFocusingModel();
	void Enter(const vec3& startPos, const vec3& endPos, const float& duration);
	void Tick(const float& deltaTime);
	bool IsExiting() const;

	vec3 GetCurrentPos() const;

	bool isEntering;

private:

	vec3 _focusingCameraStartPos;
	vec3 _focusingCameraEndPos;
	float _focusingDuration;
	float _focusingTime;
};