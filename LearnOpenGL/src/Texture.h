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

	void Bind(unsigned int slot = 0) const; //Ĭ��0��������(slot) windowsһ��32��
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};