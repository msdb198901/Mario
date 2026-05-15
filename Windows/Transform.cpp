#include "Transform.h"
#include "GameObject.h"

Transform::Transform() {
    init({0.0f, 0.0f}, {1.0f, 1.0f});
}

Transform::Transform(glm::vec2 pos) {
    init(pos, {1.0f, 1.0f});
}

Transform::Transform(glm::vec2 pos, glm::vec2 scale) {
    init(pos, scale);
}

void Transform::init(glm::vec2 pos, glm::vec2 scale) {
    this->position = pos;
    this->scale = scale;
}

json Transform::toJson() const {
    json j;
    j["x"] = position.x;
    j["y"] = position.y;
    j["scaleX"] = scale.x;
    j["scaleY"] = scale.y;
    j["rotation"] = rotation;
    return j;
}

void Transform::fromJson(const json& j) {
    position.x = j["x"];
    position.y = j["y"];
    scale.x = j["scaleX"];
    scale.y = j["scaleY"];
    rotation = j["rotation"];
    zIndex = j["zIndex"];
}

void Transform::imgui() {
    gameObject->name = JImGui::inputText("Name", gameObject->name.c_str());
    JImGui::drawVec2Control("Position", position);
    JImGui::drawVec2Control("Scale", scale, 32);

    rotation = JImGui::dragFloat("Rotation", rotation);
    zIndex = JImGui::dragInt("Z-index", zIndex);
}
