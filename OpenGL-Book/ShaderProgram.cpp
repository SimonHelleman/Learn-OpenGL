#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexSrc;
	std::string fragmentSrc;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		if (!fShaderFile.is_open())
		{
			std::cout << "not open" << std::endl;
		}

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		// Read files buffer into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexSrc = vShaderStream.str();
		fragmentSrc = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vertexSrcCStr = vertexSrc.c_str();
	const char* fragmentSrcCStr = fragmentSrc.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSrcCStr, nullptr);
	glCompileShader(vertexShader);
	CheckErrors(vertexShader, "VERTEX");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSrcCStr, nullptr);
	glCompileShader(fragmentShader);
	CheckErrors(fragmentShader, "FRAGMENT");

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);
	CheckErrors(m_ShaderProgram, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void ShaderProgram::CheckErrors(unsigned int shader, const char* type) const
{
	constexpr size_t bufferSize = 1024;
	int success;
	char infoLog[bufferSize];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, bufferSize, nullptr, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, bufferSize, nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
}
