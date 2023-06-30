#pragma once

#include <glm/ext/vector_float4.hpp>
#include <string>
using namespace std;

class Mat {

public:
	Mat(){};
	~Mat(){};

	string shaderGUID;
	string diffuseTextureGUID;
	string specularTextureGUID;
	
	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;

	void SerializeTo(const string& path);
	void DeserializeFrom(const string& path);
};
