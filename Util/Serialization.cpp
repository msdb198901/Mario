#include "Serialization.h"
#include "GameObject.h"

void to_json(json& j, const GameObject& go) {
    j = go.toJson();
}

void from_json(const json& j, std::unique_ptr<GameObject>& go) {
    go = std::make_unique<GameObject>("");
    go->fromJson(j);
}