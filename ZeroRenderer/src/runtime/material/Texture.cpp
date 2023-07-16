#include <GL/glew.h>
#include <stb_image/stb_image.h>
#include "Texture.h"
#include "GLDebug.h"

Texture::Texture(const std::string& path) {
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &width, &height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}

	this->path = path;
	std::cout << "Texture::Texture() " << m_RendererID << std::endl;
}

Texture::~Texture() {
	std::cout << "Texture::~Texture() " << m_RendererID << std::endl;
	GLCall(glDeleteTextures(1,&m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int Texture::GetID()const { return m_RendererID; }

void Texture::Bind(unsigned int slot)const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind()const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}