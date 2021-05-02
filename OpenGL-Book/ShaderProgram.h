#pragma once

#include <string>
#include <glad/glad.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);

	void Use()
	{
		glUseProgram(m_ShaderProgram);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
	}

	void SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
	}

private:
	void CheckErrors(unsigned int shader, const std::string& type);

private:
	unsigned int m_ShaderProgram;
};
