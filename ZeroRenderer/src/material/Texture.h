#pragma once 
#include <iostream>

class Texture {

private:
	unsigned int m_RendererID;
	std::string path;
	unsigned char* m_LocalBuffer;
	int width, height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	unsigned int GetID() const;

	void Bind(unsigned int slot) const;
	void UnBind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

};