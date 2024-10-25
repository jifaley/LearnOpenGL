#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string & path)
	:m_RendererId(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //垂直翻转图像。这是因为OpenGL的左下角为(0,0)点，而一般存储格式是左上角(0,0)点
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //4代表RGBA4通道

	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); //线性采样， min filter过滤器 当目标比纹理小时使用
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); //线性采样， mag filter过滤器 当目标比纹理大时使用

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //wrap_s 水平环绕 clamp嵌入， 另一选项tiling 平铺
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, m_LocalBuffer));
	//第一个0是mipmap级别

	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); //Unbind

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(unsigned int slot) const
{
	//这里的GL_TEXTURE0是枚举类型
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); //Unbind
}
