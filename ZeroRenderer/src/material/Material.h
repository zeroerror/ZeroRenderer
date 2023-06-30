#pragma once

#include <glm/ext/vector_float4.hpp>
#include <string>
#include "Shader.h"
#include "Texture.h"

using namespace std;

class Material {

public:
	Material();
	~Material();

	Shader* shader;
	Texture* diffuseTexture;

	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;

	string assetPath;
};
