#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Vertex {

public:
	Vertex() {};
	~Vertex() {};

	void SetPosition(const float& x, const float& y, const float& z);
	void SetNormal(const float& x, const float& y, const float& z);
	void SetTexCoords(const float& x, const float& y);

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

};
