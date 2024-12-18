#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererId;
	mutable std::unordered_map<std::string, int> m_LocationCache; //为了不让每次修改变量的时候查找uniform位置做的缓存
	
public:
	Shader(const std::string& filepath);
	~Shader();
	

	void Bind() const;
	void UnBind() const;

	//set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string & name, std::vector<int>& values);
	void SetUniform2i(const std::string & name, int value1, int value2);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:

	ShaderProgramSource ParseShader(const std::string & filepath);

	unsigned int CompileShader(unsigned int type, const std::string & source);

	unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

	int GetUniformLocation(const std::string& name) const;
};