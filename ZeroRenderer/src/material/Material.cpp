#include "Material.h"

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>

#include "Texture.h"

Material::Material() {
	diffuseTextureGUID = 0;
	diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	specularIntensity = 0.5f;
	shininess = 32.0f;
	shaderAssetID = 0;
	std::cout << "Material::Material() " << std::endl;
}

Material::~Material() {
	std::cout << "Material::~Material() " << std::endl;
}

void Material::SetDiffuseTexture(unsigned int diffuseTextureID) {
	this->diffuseTextureGUID = diffuseTextureID;
}

void Material::SetDiffuseColor(const glm::vec4& diffuseColor) {
	this->diffuseColor = diffuseColor;
}

void Material::SetSpecularIntensity(float specularIntensity) {
	this->specularIntensity = specularIntensity;
}

void Material::SetShininess(float shininess) {
	this->shininess = shininess;
}

void Material::SetShader(unsigned int shaderID) {
	this->shaderAssetID = shaderID;
}