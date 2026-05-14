#pragma once

#include "Component.h"
#include <nlohmann/json.hpp>
#include "JimGui.h"

class Transform : public Component {
public:
    float x = 0, y = 0, z = 0;

    // 必须实现
    std::string getTypeName() const override { return "Transform"; }
    json toJson() const override {
        return {{"x", x}, {"y", y}, {"z", z}};
    }
    void fromJson(const json& j) override {
        x = j.value("x", 0.0f);
        y = j.value("y", 0.0f);
        z = j.value("z", 0.0f);
    }

    // imgui 实现（手动）
    void imgui() override {
        JImGui::dragFloat("x", x);
        JImGui::dragFloat("y", y);
        JImGui::dragFloat("z", z);
    }
};

// 注册工厂（通常在全局或 main 之前）
inline bool registerTransform() {
    Component::registerComponent("Transform", []() { return std::make_unique<Transform>(); });
    return true;
}
static bool dummyTransform = registerTransform();