 #include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "VertexArray.h"
#include "GLDebug.h"
#include <iostream>

VertexArray::VertexArray()
	:m_RendererID(0) {
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
	std::cout << "VertexArray::~VertexArray() " << m_RendererID << std::endl;
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer* vb, const VertexBufferLayout* layout) {
	// 1. Bind the vertex array object
	GLCall(glBindVertexArray(m_RendererID));

	// 2. Bind the vertex buffer object
	vb->Bind();

	// 3. Set the layout of the vertex buffer object
	const auto& elements = layout->GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			   element.normalized, layout->GetStride(), (const void*)offset));
		offset += element.count * element.GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

