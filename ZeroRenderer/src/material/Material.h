#pragma once

#include <glm/ext/vector_float4.hpp>
#include <string>

class Material {

public:
	Material();
	~Material();

	std::string shaderGUID;
	std::string diffuseTextureGUID;
	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;

	void SetDiffuseColor(const glm::vec4& color);
	void SetSpecularIntensity(float intensity);
	void SetShininess(float value);
};
