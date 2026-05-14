#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <vector>
#include <functional>

class GameObject;
struct b2Contact;
struct b2Vec2;

using json = nlohmann::json;

// --------------------------------------------------------------
// 组件基类
// --------------------------------------------------------------
class Component {
public:
    virtual ~Component() = default;

    // 生命周期
    virtual void start() {}
    virtual void update(float dt) {}
    virtual void editorUpdate(float dt) {}

    // 碰撞回调（Box2D）
    virtual void beginCollision(GameObject* collidingObject, b2Contact* contact, const b2Vec2& hitNormal) {}
    virtual void endCollision(GameObject* collidingObject, b2Contact* contact, const b2Vec2& hitNormal) {}
    virtual void preSolve(GameObject* collidingObject, b2Contact* contact, const b2Vec2& hitNormal) {}
    virtual void postSolve(GameObject* collidingObject, b2Contact* contact, const b2Vec2& hitNormal) {}

    // 编辑器 UI（需要子类实现）
    virtual void imgui() {}

    // 序列化（多态支持）
    virtual json toJson() const = 0;
    virtual void fromJson(const json& j) = 0;

    // 获取类型名字符串（用于序列化中的 "type" 字段）
    virtual std::string getTypeName() const = 0;

    // UID 管理
    int getUid() const { return uid; }
    void generateId();
    static void init(int maxId);

    // 所属 GameObject（反序列化时会设置，不参与序列化，类似 transient）
    GameObject* gameObject = nullptr;

    // 是否参与序列化（对应 Java 的 doSerialization）
    virtual bool doSerialization() const { return true; }

    // 工厂注册（用于多态反序列化）
    using Creator = std::function<std::unique_ptr<Component>()>;
    static void registerComponent(const std::string& typeName, Creator creator);
    static std::unique_ptr<Component> createComponent(const std::string& typeName);

protected:
    int uid = -1;
    static int nextId;
};

// 全局序列化特化（让 json 能处理 Component& 和 std::unique_ptr<Component>）
void to_json(json& j, const Component& comp);
void from_json(const json& j, std::unique_ptr<Component>& comp);