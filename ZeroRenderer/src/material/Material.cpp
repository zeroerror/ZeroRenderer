#include "Material.h"

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>

#include "Texture.h"
#include "FileHelper.h"

Material::Material() {
	diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	specularIntensity = 0.5f;
	shininess = 32.0f;
	std::cout << "Material::Material() " << std::endl;
}

Material::~Material() {
	std::cout << "Material::~Material() " << std::endl;
}

