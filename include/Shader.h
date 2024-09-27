#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader 
{
public:
    GLuint ID;

    Shader(const std::string vertexPath, const std::string fragmentPath);
    ~Shader();

    // Delete copy constructor and copy assignment operator
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Allow move constructor and move assignment operator
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setFloat(const std::string& name, float value) const;
    void setBool(const std::string& name, bool value) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};
