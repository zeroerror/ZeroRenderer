#pragma once

#include <glm/ext/vector_float4.hpp>
#include <string>
using namespace std;

class Mat {

public:
	string shaderGUID;
	string diffuseTextureGUID;
	string specularTextureGUID;
	
	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;
};
