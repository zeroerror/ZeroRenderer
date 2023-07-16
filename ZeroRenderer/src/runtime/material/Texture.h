#pragma once 
#include <iostream>

class Texture {

private:
	std::string path;
	int width, height, m_BPP;

	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
public:
	Texture(const std::string& path);
	~Texture();

	unsigned int GetID() const;

	void Bind(unsigned int slot) const;
	void UnBind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

};