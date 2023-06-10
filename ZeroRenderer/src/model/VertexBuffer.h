#pragma once

class VertexBuffer {

private:
	unsigned int m_RendererID;
public:
	VertexBuffer();
	~VertexBuffer();
	void Ctor(const void* data, unsigned int size);

	void Bind() const;
	void UnBind() const;

};