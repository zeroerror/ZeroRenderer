#pragma once
#include "ComponentMeta.h"
#include "Camera.h"

class CameraMeta :public ComponentMeta
{
public:

	CameraMeta();
	~CameraMeta();
	
	CameraType cameraType;
	float nearPlane;
	float farPlane;
	float fov;
	float scrWidth;
	float scrHeight;
	float orthoSize;

	void ToCamera(Camera* camera);
};

