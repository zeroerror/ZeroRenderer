#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "CameraType.h"
#include "Component.h"
#include "Transform.h"

class Camera : public Component
{
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

	void Update(const float &dt);

	/**
	 * 获取相机的正交MVP矩阵
	 * @param modelPos 模型位置
	 * @return MVP矩阵
	 */
	glm::mat4 GetMVPMatrix_Ortho(const glm::vec3 &modelPos) const;

	/**
	 * 获取相机的透视MVP矩阵
	 * @param modelPos 模型位置
	 * @return MVP矩阵
	 */
	glm::mat4 GetMVPMatrix_Perspective(const glm::vec3 &modelPos) const;

	/**
	 * 从另一个相机拷贝数据
	 */
	void CopyFrom(const Camera &camera);

	/**
	 * 获取正交MVP矩阵
	 * @param modelPos 模型位置
	 * @param trans 模型的变换组件
	 * @param orthoSize 正交大小
	 * @param nearPlane 近裁剪面
	 * @param farPlane 远裁剪面
	 * @return MVP矩阵
	 */
	static glm::mat4 GetMVPMatrix_Ortho(const glm::vec3 &modelPos, const Transform &trans, const float &orthoSize, const float &nearPlane, const float &farPlane);
};
