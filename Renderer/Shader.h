#pragma once 

#include <string>
#include <glm/glm.hpp>

#include <vector>

class Shader {
    public:
        Shader(const char* filePath);
        ~Shader();

        void compile();
        void use();
        void detach();

        void uploadMat4f(const char* varName, const glm::mat4& matrix);
        void uploadMat3f(const char* varName, const glm::mat3& matrix);

        void uploadVec2f(const char* varName, const glm::vec2& vector);
        void uploadVec3f(const char* varName, const glm::vec3& vector);
        void uploadVec4f(const char* varName, const glm::vec4& vector);

        void uploadFloat(const char* varName, float value);
        void uploadInt(const char* varName, int value);
        void uploadBool(const char* varName, bool value);
        void uploadTexture(const char* varName, int slot);
        void uploadIntArray(const char* varName, std::vector<int> values);

        int getID() const { return shaderProgramID; }

    private:
        unsigned int shaderProgramID;
        bool isBeingUsed = false;

        std::string vertexSource;
        std::string fragmentSource;
        std::string filePath;
};