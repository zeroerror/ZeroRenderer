#pragma once
#include "ComponentMeta.h"
#include "CameraType.h"

class CameraMeta :public ComponentMeta
{
public:
	CameraType cameraType;
	float scrWidth;
	float scrHeight;
	float fov;
	float nearPlane;
	float farPlane;
	float orthoSize;
};

