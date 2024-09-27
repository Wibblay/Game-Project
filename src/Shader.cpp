#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string vertexPath, const std::string fragmentPath) 
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try 
    {
        // Open files
        vShaderFile.open(vertexPath);
        if (!vShaderFile.is_open()) 
        {
            std::cerr << "ERROR::SHADER::VERTEX_SHADER_FILE_NOT_OPENED: " << vertexPath << "\n";
        }

        fShaderFile.open(fragmentPath);
        if (!fShaderFile.is_open()) 
        {
            std::cerr << "ERROR::SHADER::FRAGMENT_SHADER_FILE_NOT_OPENED: " << fragmentPath << "\n";
        }

        // Read file's buffer contents into streams
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } 
    catch (std::ifstream::failure& e) 
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_READ\n";
        std::cerr << "Exception message: " << e.what() << "\n";
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete shaders as they're linked
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() 
{
    glDeleteProgram(ID);
}

Shader::Shader(Shader&& other) noexcept
    : ID(other.ID) 
    {
    other.ID = 0; // Reset other's ID to prevent deletion
}

Shader& Shader::operator=(Shader&& other) noexcept 
{
    if (this != &other) 
    {
        glDeleteProgram(ID); // Delete current program
        ID = other.ID;
        other.ID = 0; // Reset other's ID
    }
    return *this;
}

void Shader::use() const 
{
    glUseProgram(ID);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const 
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) 
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    } 
    else 
    {
        std::cerr << "Uniform '" << name << "' not found in shader.\n";
    }
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const 
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) 
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
    } 
    else 
    {
        std::cerr << "Uniform '" << name << "' not found in shader.\n";
    }
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const 
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) 
    {
        glUniform2fv(location, 1, &value[0]);
    } 
    else 
    {
        std::cerr << "Uniform '" << name << "' not found in shader.\n";
    }
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const 
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) 
    {
        glUniform3fv(location, 1, &value[0]);
    } 
    else 
    {
        std::cerr << "Uniform '" << name << "' not found in shader.\n";
    }
}

void Shader::setFloat(const std::string& name, float value) const 
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) 
    {
        glUniform1f(location, value);
    } 
    else 
    {
        std::cerr << "Uniform '" << name << "' not found in shader.\n";
    }
}

void Shader::setBool(const std::string& name, bool value) const
{
   GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) 
    {
        glUniform1i(location, static_cast<int>(value));
    } 
    else 
    {
        std::cerr << "Uniform '" << name << "' not found in shader.\n";
    } 
}

void Shader::checkCompileErrors(GLuint shader, std::string type) 
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") 
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n";
        }
    } 
    else 
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n";
        }
    }
}
