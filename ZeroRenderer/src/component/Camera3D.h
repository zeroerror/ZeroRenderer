#pragma once

#include "Transform.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

class Camera3D {
public:
	Camera3D();
	~Camera3D();

	void Update(const float& dt);
	void Render(glm::vec3 modPosition, glm::quat modRotation, Material* material, VertexArray* va, IndexBuffer* ib);

	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3& pos, const glm::quat& rot);
	glm::mat4 GetMVPMatrix_Perspective(const glm::vec3& pos, const glm::quat& rot);


public:
	Transform transform;
	float width;
	float height;
	float depth;


};
