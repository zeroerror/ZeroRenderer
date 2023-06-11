#pragma once

class IndexBuffer {

private:
	unsigned int m_RendererID;
	unsigned int m_count;
public:
	IndexBuffer();
	~IndexBuffer();

	void Ctor(const unsigned int* data, unsigned int count);
	void Bind() const;
	void UnBind() const;
	unsigned int GetCount() const;
};