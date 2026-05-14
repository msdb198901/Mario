#include "Component.h"
#include <stdexcept>

int Component::nextId = 0;

void Component::generateId() {
    if (uid == -1) uid = nextId++;
}

void Component::init(int maxId) {
    nextId = maxId;
}

// ---------- 工厂实现 ----------
static std::unordered_map<std::string, Component::Creator>& getRegistry() {
    static std::unordered_map<std::string, Component::Creator> registry;
    return registry;
}

void Component::registerComponent(const std::string& typeName, Creator creator) {
    getRegistry()[typeName] = std::move(creator);
}

std::unique_ptr<Component> Component::createComponent(const std::string& typeName) {
    auto it = getRegistry().find(typeName);
    if (it != getRegistry().end()) {
        return it->second();
    }
    throw std::runtime_error("Unknown component type: " + typeName);
}

// ---------- 全局序列化 ----------
void to_json(json& j, const Component& comp) {
    j = {
        {"type", comp.getTypeName()},
        {"properties", comp.toJson()}
    };
}

void from_json(const json& j, std::unique_ptr<Component>& comp) {
    std::string typeName = j.at("type");
    comp = Component::createComponent(typeName);
    if (comp) {
        comp->fromJson(j.at("properties"));
    }
}