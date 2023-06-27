#pragma once
#include <string>
#include <glm/ext/vector_float4.hpp>
#include <iostream>
#include <fstream>

class MaterialMetadata {

public:
	MaterialMetadata(){};
	~MaterialMetadata(){};

	std::string path;
	std::string shaderGUID;
	std::string diffuseTextureGUID;
	glm::vec4 diffuseColor;
	float specularIntensity;
	float shininess;

	void SerializeTo();
	void DeserializeFrom();

};

