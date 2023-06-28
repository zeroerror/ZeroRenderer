#pragma once

#include <glm/ext/vector_float4.hpp>
#include <string>
using namespace std;

class Material {

public:
	Material();
	~Material();

	string shaderGUID;
	string diffuseTextureGUID;

	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;
};
