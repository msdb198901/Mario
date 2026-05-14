#include "JimGui.h"


float JImGui::defaultColumnWidth = 220.0f;

void JImGui::drawVec2Control(const char* name, glm::vec2& vec) {
    drawVec2Control(name, vec, 0.0f, defaultColumnWidth);
}

void JImGui::drawVec2Control(const char* name, glm::vec2& vec, float resetValue) {
    drawVec2Control(name, vec, resetValue, defaultColumnWidth);
}

void JImGui::drawVec2Control(const char* name, glm::vec2& vec, float resetValue, float columnWidth) {
    ImGui::PushID(name); // Ensure unique ID for this control

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name);
    ImGui::NextColumn();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 2) / 2;

    ImGui::PushItemWidth(widthEach);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});

    if (ImGui::Button("X", buttonSize)) {
        vec.x = resetValue;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &vec.x, 0.01f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(widthEach);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});


    if (ImGui::Button("Y", buttonSize)) {
        vec.y = resetValue;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &vec.y, 0.01f);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::NextColumn();

    // values.x = vec.x;
    // values.y = vec.y;

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}

void JImGui::drawVec3Control(const char* name, glm::vec3& vec) {
    drawVec3Control(name, vec, 0.0f, defaultColumnWidth);
}

void JImGui::drawVec3Control(const char* name, glm::vec3& vec, float resetValue) {
    drawVec3Control(name, vec, resetValue, defaultColumnWidth);
}

void JImGui::drawVec3Control(const char* name, glm::vec3& vec, float resetValue, float columnWidth) {
    ImGui::PushID(name); // Ensure unique ID for this control

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name);
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 3) / 3;

    ImGui::PushItemWidth(widthEach);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize)) {
        vec.x = resetValue;
    }

    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &vec.x, 0.01f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(widthEach);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize)) {
        vec.y = resetValue;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &vec.y, 0.01f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(widthEach);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.5f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.6f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.5f, 1.0f});
    if (ImGui::Button("Z", buttonSize)) {
        vec.z = resetValue;
    }
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &vec.z, 0.01f);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

    // values.x = vec.x;
    // values.y = vec.y;
    // values.z = vec.z;

    ImGui::PopStyleVar();
    ImGui::PopID();
}

float JImGui::dragFloat(const char* label, float& value) {
    ImGui::PushID(label);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, defaultColumnWidth);
    ImGui::Text(label);
    ImGui::NextColumn();

    ImGui::DragFloat("##DragFloat", &value, 0.01f);

    ImGui::Columns(1);
    ImGui::PopID();
    return value;
}

int JImGui::dragInt(const char* label, int& value) {
    ImGui::PushID(label);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, defaultColumnWidth);
    ImGui::Text(label);
    ImGui::NextColumn();

    ImGui::DragInt("##DragInt", &value, 1);

    ImGui::Columns(1);
    ImGui::PopID();
    return (int)value;
}

bool JImGui::colorPicker4(const char* label, glm::vec4& value) {
    bool changed = false;

    ImGui::PushID(label);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, defaultColumnWidth);
    ImGui::Text(label);
    ImGui::NextColumn();


    float color[4] = { value.x, value.y, value.z, value.w };
    if (ImGui::ColorEdit4("##ColorPicker4", color)) {
        value.x = color[0];
        value.y = color[1];
        value.z = color[2];
        value.w = color[3];
        changed = true;
    }

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}

const char* JImGui::inputText(const char* label, const char* str) {
    ImGui::PushID(label);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, defaultColumnWidth);
    ImGui::Text(label);
    ImGui::NextColumn();

    static char buffer[256];
    strncpy(buffer, str, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

    if (ImGui::InputText("##InputText", buffer, sizeof(buffer))) {
        // Return a pointer to the modified string
        ImGui::Columns(1);
        ImGui::PopID();
        return buffer;
    }

    ImGui::Columns(1);
    ImGui::PopID();
    return str; // Return original if not modified
}