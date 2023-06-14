#pragma once

#include "Transform.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

class Camera3D {
public:
	Camera3D();
	~Camera3D();

	void Update(const float& dt);
	void Render(const glm::vec3 modPosition, const glm::quat modRotation, const VertexArray* va, const IndexBuffer* ib, const Shader* shader, const Texture* texture);

	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3& pos, const glm::quat& rot);
	glm::mat4 GetMVPMatrix_Perspective(const glm::vec3& pos);


public:
	Transform* transform;
	float width;
	float height;
	float nearPlane;
	float farPlane;

};
