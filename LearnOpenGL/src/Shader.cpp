#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "utils.h"

Shader::Shader(const std::string & filepath):m_FilePath(filepath), m_RendererId(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	std::cout << "Vertex: " << std::endl;
	std::cout << source.vertexSource << std::endl;
	std::cout << "Fragment: " << std::endl;
	std::cout << source.fragmentSource << std::endl;

	m_RendererId = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererId));
}



ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		//据说alloca是在栈上申请内存的，不需要手动free
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile"
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
	}


	return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//glDeleteShader(vs);
	//glDeleteShader(fs);
	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererId));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string & name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string & name, std::vector<int>& values)
{
	GLCall(glUniform1iv(GetUniformLocation(name), values.size(), &values[0]));
}

void Shader::SetUniform2i(const std::string & name, int value1, int value2)
{
	GLCall(glUniform2i(GetUniformLocation(name), value1, value2));
}

void Shader::SetUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string & name, const glm::mat4 & matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


int Shader::GetUniformLocation(const std::string & name) const
{
	if (m_LocationCache.find(name) != m_LocationCache.end())
		return m_LocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist in the cache!" << std::endl;
	
	m_LocationCache[name] = location;
	return location;
}