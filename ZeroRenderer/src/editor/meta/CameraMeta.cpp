#include "CameraMeta.h"

CameraMeta::CameraMeta() {
	componentType = ComponentType_Camera;
}

CameraMeta::~CameraMeta() 
{

}

void CameraMeta::ToCamera(Camera* camera) {
	camera->cameraType = cameraType;
	camera->nearPlane = nearPlane;
	camera->farPlane = farPlane;
	camera->fov = fov;
	camera->orthoSize = orthoSize;
	camera->scrWidth = scrWidth;
	camera->scrHeight = scrHeight;
}