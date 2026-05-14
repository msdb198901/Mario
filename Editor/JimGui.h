#pragma once 

#include <glm/glm.hpp>
#include <imgui.h>

#define IMGUI_VEC2(name, vec)  JImGui::drawVec2Control(name, vec)   // 需实现
#define IMGUI_VEC3(name, vec)  do { float v[3] = {vec.x, vec.y, vec.z}; if (ImGui::DragFloat3(name, v)) vec.set(v[0], v[1], v[2]); } while(0)
#define IMGUI_VEC4(name, vec)  JImGui::colorPicker4(name, vec)

class JImGui {
    public:
        static void drawVec2Control(const char* name, glm::vec2& vec);
        static void drawVec2Control(const char* name, glm::vec2& vec, float resetValue);
        static void drawVec2Control(const char* name, glm::vec2& vec, float resetValue, float columnWidth);
        static void drawVec3Control(const char* name, glm::vec3& vec);
        static void drawVec3Control(const char* name, glm::vec3& vec, float resetValue);
        static void drawVec3Control(const char* name, glm::vec3& vec, float resetValue, float columnWidth);
        static float dragFloat(const char* label, float& value);
        static int dragInt(const char* label, int& value);
        static bool colorPicker4(const char* label, glm::vec4& value);
        static const char* inputText(const char* label, const char* str);
    private:
        static float defaultColumnWidth;
};