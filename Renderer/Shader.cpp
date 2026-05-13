#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <cassert>

Shader::Shader(const char* filepath) {
    // 1. 读取整个文件为字符串
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for shader: '" << filepath << "'" << std::endl;
        assert(false);
    }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // 2. 提取两个 #type 后面的模式名称 (vertex / fragment)
    std::regex typeRegex(R"(#type\s+([a-zA-Z]+))");
    std::vector<std::string> patterns;
    std::sregex_iterator it(source.begin(), source.end(), typeRegex);
    std::sregex_iterator end;
    for (; it != end; ++it) {
        patterns.push_back((*it)[1].str());
    }
    if (patterns.size() < 2) {
        std::cerr << "Error: Shader file must contain at least two #type directives" << std::endl;
        assert(false);
    }
    std::string firstPattern = patterns[0];
    std::string secondPattern = patterns[1];

    // 3. 按 #type... 分割源码，获取每个着色器体
    std::regex sepRegex(R"(#type\s+[a-zA-Z]+)");
    std::sregex_token_iterator splitIt(source.begin(), source.end(), sepRegex, -1);
    std::sregex_token_iterator splitEnd;
    std::vector<std::string> tokens;
    for (; splitIt != splitEnd; ++splitIt) {
        tokens.push_back(*splitIt);
    }
    if (tokens.size() < 3) {  // tokens[0] 是第一个 #type 前的内容，[1] 是中间，[2] 是第二个 #type 后
        std::cerr << "Error: Unexpected shader file format" << std::endl;
        assert(false);
    }
    std::string source1 = tokens[1];   // 第一个着色器源码
    std::string source2 = tokens[2];   // 第二个着色器源码

    // 4. 根据模式名称分配源码
    if (firstPattern == "vertex") {
        vertexSource = source1;
    } else if (firstPattern == "fragment") {
        fragmentSource = source1;
    } else {
        std::cerr << "Unexpected token '" << firstPattern << "'" << std::endl;
        assert(false);
    }

    if (secondPattern == "vertex") {
        vertexSource = source2;
    } else if (secondPattern == "fragment") {
        fragmentSource = source2;
    } else {
        std::cerr << "Unexpected token '" << secondPattern << "'" << std::endl;
        assert(false);
    }
}

Shader::~Shader() {
    glDeleteProgram(shaderProgramID);
}

void Shader::use() {
    if (!isBeingUsed) {
        glUseProgram(shaderProgramID);
        isBeingUsed = true;
    }
}

void Shader::detach() {
    glUseProgram(0);
    isBeingUsed = false;
}

void Shader::uploadMat4f(const char* varName, const glm::mat4& matrix) {
    use();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, varName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadMat3f(const char* varName, const glm::mat3& matrix) {
    use();
    glUniformMatrix3fv(glGetUniformLocation(shaderProgramID, varName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadVec2f(const char* varName, const glm::vec2& vector) {
    use();
    glUniform2f(glGetUniformLocation(shaderProgramID, varName), vector.x, vector.y);
}

void Shader::uploadVec3f(const char* varName, const glm::vec3& vector) {
    use();
    glUniform3f(glGetUniformLocation(shaderProgramID, varName), vector.x, vector.y, vector.z);
}

void Shader::uploadVec4f(const char* varName, const glm::vec4& vector) {
    use();
    glUniform4f(glGetUniformLocation(shaderProgramID, varName), vector.x, vector.y, vector.z, vector.w);
}

void Shader::uploadFloat(const char* varName, float value) {
    use();
    glUniform1f(glGetUniformLocation(shaderProgramID, varName), value);
}

void Shader::uploadInt(const char* varName, int value) {
    use();
    glUniform1i(glGetUniformLocation(shaderProgramID, varName), value);
}

void Shader::uploadBool(const char* varName, bool value) {
    use();
    glUniform1i(glGetUniformLocation(shaderProgramID, varName), value);
}

void Shader::uploadTexture(const char* varName, int slot) {
    use();
    glUniform1i(glGetUniformLocation(shaderProgramID, varName), slot);
}

void Shader::uploadIntArray(const char* varName, int count, const int* values) {
    use();
    glUniform1iv(glGetUniformLocation(shaderProgramID, varName), count, values);
}

void Shader::compile() {
    const char* vShaderCode = vertexSource.c_str();
    const char* fShaderCode = fragmentSource.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertex);
    glAttachShader(shaderProgramID, fragment);
    glLinkProgram(shaderProgramID);
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}