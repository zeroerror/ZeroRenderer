#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "CameraType.h"
#include "Component.h"
#include "Transform.h"

class Camera : public Component{
public:
	CameraType cameraType;
	float scrWidth;
	float scrHeight;
	float fov;
	float nearPlane;
	float farPlane;
	float orthoSize;

	Camera();
	~Camera();
	
	void Update(const float& dt);
	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3& pos) const;
	glm::mat4 GetMVPMatrix_Perspective(const glm::vec3& pos) const;

};
