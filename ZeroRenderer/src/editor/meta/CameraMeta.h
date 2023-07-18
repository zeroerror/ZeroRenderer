#pragma once
#include "ComponentMeta.h"
#include "Camera.h"

class CameraMeta :public ComponentMeta
{
public:
	CameraMeta() {
		nearPlane = 0.1f;
		farPlane = 100.0f;
		fov = 45.0f;
		orthoSize = 20.0f;
		cameraType = CameraType::Perspective;

		scrWidth = 1920;
		scrHeight = 1080;
		componentType = ComponentType_Camera;
	}

	CameraType cameraType;
	float nearPlane;
	float farPlane;
	float fov;
	float scrWidth;
	float scrHeight;
	float orthoSize;

};

