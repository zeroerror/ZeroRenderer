#pragma once

#include "Transform.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "CameraType.h"

class Camera3D {
public:
	Camera3D();
	~Camera3D();

	void Update(const float& dt);

	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3& pos);
	glm::mat4 GetMVPMatrix_Perspective(const glm::vec3& pos);


public:
	Transform* transform;
	CameraType cameraType;
	float scrWidth;
	float scrHeight;
	float orthoSize;
	float nearPlane;
	float farPlane;

};
