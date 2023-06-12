#include "VertexBuffer.h"
#include <GL/glew.h>
#include <iostream>

VertexBuffer::VertexBuffer() {
}

VertexBuffer::~VertexBuffer() {
	std::cout << "VertexBuffer::~VertexBuffer() " << m_RendererID << std::endl;
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Ctor(const void* data, unsigned int size) {
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}