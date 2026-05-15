#include "GameObject.h"
#include "Transform.h"      // 具体组件头文件
#include "Serialization.h"  // 全局序列化特化（GameObject 的 to_json/from_json）
#include "SpriteRenderer.h"
#include "Texture.h"
#include "AssetPool.h"

#include <nlohmann/json.hpp>

int GameObject::nextId = 0;

GameObject::GameObject(const std::string& name) : name(name) {
    uid = nextId++;
}

void GameObject::generateUid() {
    uid = nextId++;
}

void GameObject::init(int maxId) {
    nextId = maxId;
}

void GameObject::addComponent(std::unique_ptr<Component> comp) {
    if (comp) {
        comp->generateId();
        comp->gameObject = this;
        components.push_back(std::move(comp));
    }
}

std::vector<Component*> GameObject::getAllComponents() const {
    std::vector<Component*> result;
    result.reserve(components.size());
    for (auto& c : components) result.push_back(c.get());
    return result;
}

void GameObject::start() {
    for (auto& c : components) c->start();
}

void GameObject::update(float dt) {
    for (auto& c : components) c->update(dt);
}

void GameObject::editorUpdate(float dt) {
    for (auto& c : components) c->editorUpdate(dt);
}

void GameObject::imgui() {
    // for (auto& c : components) {
    //     if (ImGui::CollapsingHeader(c->getTypeName().c_str())) {
    //         c->imgui();
    //     }
    // }
}

void GameObject::destroy() {
    // dead = true;
    // for (auto& c : components) c->destroy();
}

json GameObject::toJson() const {
    json j;
    j["name"] = name;
    j["uid"] = uid;
    j["components"] = json::array();
    for (auto& c : components) {
        j["components"].push_back(*c);   // 调用全局 to_json(Component&)
    }
    return j;
}

void GameObject::fromJson(const json& j) {
    name = j.at("name");
    if (j.contains("uid")) uid = j["uid"];
    else uid = nextId++;  // fallback

    if (j.contains("components") && j["components"].is_array()) {
        for (auto& compJson : j["components"]) {
            std::unique_ptr<Component> comp;
            from_json(compJson, comp);   // 调用全局 from_json
            if (comp) addComponent(std::move(comp));
        }
    }

    // 设置 transform 指针（类似 Java 中的 go.transform = go.getComponent(Transform.class);）
    // transform = getComponent<Transform>();
}

GameObject GameObject::copy() const {
    // 序列化当前对象到 JSON
    json j = this->toJson();
    // 反序列化成新对象
    GameObject copy("");
    copy.fromJson(j);

    // 重新生成 UID（类似 Java 中的 obj.generateUid()）
    copy.generateUid();
    for (auto* c : copy.getAllComponents()) {
        c->generateId();
    }

    // 处理资源重载（例如 SpriteRenderer 的纹理需要从 AssetPool 重新获取）
    if (auto* sprite = copy.getComponent<SpriteRenderer>()) {
        if (sprite->getTexture() && !sprite->getTexture()->getFilepath().empty()) {
            sprite->setTexture(AssetPool::getTexture(sprite->getTexture()->getFilepath()));
        }
    }
    return copy;
}