
#include "pch.h"
#include <glad\glad.h>
#include "Shader.h"
#include <glm\gtc\type_ptr.hpp>

namespace XRE {
	Shader::Shader(const std::string vertexPath, const std::string fragmentPath):
        m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
	{
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            XRE_CORE_ERROR("SHADER_FILE_NOT_SUCCESSFULLY_READ:{0}", e.what());
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertex);
        glAttachShader(m_RendererID, fragment);
        glLinkProgram(m_RendererID);
        checkCompileErrors(m_RendererID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}
    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }
	void Shader::Bind() const
	{
        glUseProgram(m_RendererID);
	}
	void Shader::UnBind() const
	{
        glUseProgram(0);
	}
	void Shader::setBool(const std::string& name, bool value) const
	{
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}
	void Shader::setInt(const std::string& name, int value) const
	{
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	void Shader::setFloat(const std::string& name, float value) const
	{
        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
    void Shader::setMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
	void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::string errFile;
                if (type == "VERTEX") errFile = m_VertexPath;
                else if (type == "FRAGMENT") errFile = m_FragmentPath;
                XRE_CORE_ERROR("SHADER_COMPILATION_ERROR of {0} in {1}", type, errFile);
                
                XRE_CORE_ERROR("{0}", infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                XRE_CORE_ERROR("PROGRAM_LINKING_ERROR of type:{0}", type);
                XRE_CORE_ERROR("{0}", infoLog);
                
            }
        }
	}
}