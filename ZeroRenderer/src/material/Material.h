#pragma once

#include <glm/ext/vector_float4.hpp>

class Material {

public:
	Material();
	~Material();

	unsigned int diffuseTextureAssetID;
	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;
	unsigned int shaderAssetID;

	void SetDiffuseTexture(unsigned int textureAssetID);
	void SetDiffuseColor(const glm::vec4& color);
	void SetSpecularIntensity(float intensity);
	void SetShininess(float value);
	void SetShader(unsigned int shaderAssetID);
};
