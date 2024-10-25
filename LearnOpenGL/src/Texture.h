#pragma once
#include "utils.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererId;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; //bits per pixel
	
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const; //默认0号纹理插槽(slot) windows一般32个
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};