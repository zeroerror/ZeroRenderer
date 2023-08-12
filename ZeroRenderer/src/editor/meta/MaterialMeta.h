#pragma once 

#include <glm/vec4.hpp>
#include <string>

using namespace std;
using namespace glm;

class MaterialMeta {

public:
	string shaderGUID;
	string diffuseTextureGUID;
	string specularTextureGUID;

	vec4 diffuseColor;
	float specularIntensity;
	float shininess;

	string guid;

};