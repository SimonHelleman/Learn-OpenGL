#pragma once

#include <string>
#include <glad/glad.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);

	void Use() const
	{
		glUseProgram(m_ShaderProgram);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
	}

	// This will be faster if a literal is passed
	void SetInt(const const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgram, name), value);
	}

	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
	}

	void SetFloat(const char* name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgram, name), value);
	}

private:
	void CheckErrors(unsigned int shader, const char* type) const;

private:
	unsigned int m_ShaderProgram;
};
