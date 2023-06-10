#pragma once

#include <glm/ext/vector_float4.hpp>
#include <Texture.h>
#include <Shader.h>

class Material {

public:
	Material();

	Texture* diffuseTexture;
	Shader* shader;
	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;

	void SetDiffuseTexture(Texture* texture);
	void SetDiffuseColor(const glm::vec4& color);
	void SetSpecularIntensity(float intensity);
	void SetShininess(float value);
	void SetShader(Shader* shader);
	void Bind() const;
};
