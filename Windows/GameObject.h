#pragma once

#include "Component.h"
#include <vector>
#include <string>
#include <memory>

class Transform; // 前向声明

class GameObject {
public:
    explicit GameObject(const std::string& name = "");
    ~GameObject() = default;

    // 禁止拷贝，支持移动
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    // 组件管理
    template<typename T>
    T* getComponent();
    template<typename T>
    void removeComponent();
    void addComponent(std::unique_ptr<Component> comp);
    std::vector<Component*> getAllComponents() const;

    // 生命周期
    void start();
    void update(float dt);
    void editorUpdate(float dt);
    void imgui();    // 遍历所有组件的 imgui
    void destroy();

    // 复制（通过 JSON 深拷贝）
    GameObject copy() const;

    // UID
    int getUid() const { return uid; }
    void generateUid();
    static void init(int maxId);

    // 序列化控制
    bool doSerialization() const { return isDoSerialization; }
    void setNoSerialize() { isDoSerialization = false; }

    // 序列化接口
    json toJson() const;
    void fromJson(const json& j);

    // 成员变量
    std::string name;
    Transform* transform = nullptr;   // 类似 Java 中的 transient 指针
    bool isDead() const { return dead; }

private:
    int uid = -1;
    bool dead = false;
    bool isDoSerialization = true;
    std::vector<std::unique_ptr<Component>> components;

    static int nextId;
};

// 需要在 GameObject.cpp 中显式实例化常用模板，或直接在头文件中实现
template<typename T>
T* GameObject::getComponent() {
    for (auto& comp : components) {
        if (auto* ptr = dynamic_cast<T*>(comp.get())) {
            return ptr;
        }
    }
    return nullptr;
}

template<typename T>
void GameObject::removeComponent() {
    for (auto it = components.begin(); it != components.end(); ++it) {
        if (dynamic_cast<T*>(it->get())) {
            components.erase(it);
            return;
        }
    }
}