#pragma once
#include "ComponentMeta.h"
#include "Camera.h"

class CameraMeta :public ComponentMeta
{
public:
	CameraMeta() { componentType = ComponentType_Camera; }
	
	CameraType cameraType;
	float nearPlane;
	float farPlane;
	float fov;
	float scrWidth;
	float scrHeight;
	float orthoSize;

};

