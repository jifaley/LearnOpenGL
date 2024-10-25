#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string & path)
	:m_RendererId(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //��ֱ��תͼ��������ΪOpenGL�����½�Ϊ(0,0)�㣬��һ��洢��ʽ�����Ͻ�(0,0)��
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //4����RGBA4ͨ��

	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); //���Բ����� min filter������ ��Ŀ�������Сʱʹ��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); //���Բ����� mag filter������ ��Ŀ��������ʱʹ��

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //wrap_s ˮƽ���� clampǶ�룬 ��һѡ��tiling ƽ��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, m_LocalBuffer));
	//��һ��0��mipmap����

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
	//�����GL_TEXTURE0��ö������
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); //Unbind
}
