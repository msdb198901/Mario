#pragma once

#include <nlohmann/json.hpp>
#include <memory>

class GameObject;
class Component;

// 让 GameObject 支持 json 转换
void to_json(nlohmann::json& j, const GameObject& go);
void from_json(const nlohmann::json& j, std::unique_ptr<GameObject>& go);