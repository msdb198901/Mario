#pragma once

#include "Component.h"
#include <nlohmann/json.hpp>
#include "JimGui.h"

class Transform : public Component {
public:
    float x = 0, y = 0, z = 0;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation = 0;
    int zIndex = 0;

    Transform();
    Transform(glm::vec2 pos);
    Transform(glm::vec2 pos, glm::vec2 scale = {1.0f, 1.0f});

    void init(glm::vec2 pos, glm::vec2 scale = {1.0f, 1.0f});

    // 必须实现
    std::string getTypeName() const override { return "Transform"; }
    json toJson() const override;
    void fromJson(const json& j) override;

    // imgui 实现（手动）
    void imgui() override;
};

// 注册工厂（通常在全局或 main 之前）
inline bool registerTransform() {
    Component::registerComponent("Transform", []() { return std::make_unique<Transform>(); });
    return true;
}
static bool dummyTransform = registerTransform();