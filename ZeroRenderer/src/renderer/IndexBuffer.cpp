#include "IndexBuffer.h"
#include <GLDebug.h>
#include <GL/glew.h>
#include <iostream>

IndexBuffer::IndexBuffer()
	:m_RendererID(0), m_count(0)
{
}

IndexBuffer::~IndexBuffer() {
	std::cout << "IndexBuffer::~IndexBuffer() " << m_RendererID << std::endl;

	if (m_RendererID != 0) {
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}
}


void IndexBuffer::Ctor(const unsigned int* data, unsigned int count) {
	m_count = count;
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::GetCount() const {
	return m_count;
}

