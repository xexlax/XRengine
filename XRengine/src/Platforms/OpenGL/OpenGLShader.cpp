#include "pch.h"
#include "OpenGLShader.h"
#include "glad\glad.h"
#include <glm\gtc\type_ptr.hpp>

namespace XRE{
static GLenum ShaderTypeFromString(const std::string& type)
{   
    //判断shader类型
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    if (type == "geometry")
        return GL_GEOMETRY_SHADER;

    XRE_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}
}
//双文件构造函数
XRE::OpenGLShader::OpenGLShader(const std::string& name , const std::string& vertexPath, const std::string& fragmentPath):
   m_Name(name), m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath 
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode="";
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

    Compile(vertexCode, fragmentCode,geometryCode);
    
}

//单文件(.glsl)构造函数
XRE::OpenGLShader::OpenGLShader(const std::string& filepath):
    m_VertexPath(filepath),m_FragmentPath(filepath)
{
    std::string source = ReadFile(filepath);
    
    std::string vertex_code, fragment_code, geometry_code;
    PreProcess(source,vertex_code,fragment_code, geometry_code);
    Compile(vertex_code, fragment_code, geometry_code);

    // Extract name from filepath
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
}

XRE::OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_RendererID);
}

void XRE::OpenGLShader::Bind() const 
{
    glUseProgram(m_RendererID);
}

void XRE::OpenGLShader::UnBind() const
{
    glUseProgram(0);
}

void XRE::OpenGLShader::SetBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
}

void XRE::OpenGLShader::SetInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void XRE::OpenGLShader::SetFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void XRE::OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void XRE::OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void XRE::OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}


void XRE::OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void XRE::OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string XRE::OpenGLShader::ReadFile(const std::string& filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        XRE_CORE_ERROR("Could not open file '{0}'", filepath);
    }

    return result;
}
//查找文件中的#type标签，分开储存在map中
void XRE::OpenGLShader::PreProcess(const std::string& source, std::string& vc, std::string& fc, std::string& gc)
{
    

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        XRE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        XRE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        std::string code = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        
        if (ShaderTypeFromString(type) == GL_VERTEX_SHADER)vc = code;
        else if (ShaderTypeFromString(type) == GL_FRAGMENT_SHADER)fc = code;
        else if (ShaderTypeFromString(type) == GL_GEOMETRY_SHADER)gc = code;
    
    }
    
}

void XRE::OpenGLShader::Compile(const std::string& vc, const std::string& fc, const std::string& gc)
{
    const char* vShaderCode = vc.c_str();
    const char* fShaderCode = fc.c_str();
    const char* gShaderCode = gc.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment,geometry;
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
    //geometry Shader

    if (gc != "") {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }



    // shader Program
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertex);
    

    if (gc != "") {
        glAttachShader(m_RendererID, geometry);
    }

    glAttachShader(m_RendererID, fragment);

    glLinkProgram(m_RendererID);
    checkCompileErrors(m_RendererID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    if (gc != "") {
        glDeleteShader(geometry);
    }
    glDeleteShader(fragment);

}

void XRE::OpenGLShader::checkCompileErrors(unsigned int shader, std::string type)
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
            else if (type == "GEOMETRY") errFile = m_FragmentPath;
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
