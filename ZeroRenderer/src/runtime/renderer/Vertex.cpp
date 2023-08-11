#include "Vertex.h"

Vertex::Vertex(){};

Vertex::Vertex(const vec3& pos, const vec3& nor, const vec2& tex) {
	this->position = pos;
	this->normal = nor;
	this->texCoords = tex;
};

Vertex::~Vertex() {};

void Vertex::SetPosition(const float& x, const float& y, const float& z) {
	this->position = glm::vec3(x, y, z);
}

void Vertex::SetNormal(const float& x, const float& y, const float& z) {
	this->normal = glm::vec3(x, y, z);
}

void Vertex::SetTexCoords(const float& x, const float& y) {
	this->texCoords = glm::vec2(x, y);
}