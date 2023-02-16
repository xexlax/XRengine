#pragma once
#include <string>
#include <glm/glm.hpp>

namespace XRE {
    class Shader
    {
    public:
        Shader(const std::string vertexPath, const std::string fragmentPath);
        ~Shader();
        void Bind() const;
        void UnBind() const;


        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, const glm::mat4& matrix);

    private:
        unsigned int m_RendererID;
        void checkCompileErrors(unsigned int shader, std::string type);
        //for logging errors
        std::string m_VertexPath,m_FragmentPath;
          
    };
}