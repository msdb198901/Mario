#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

#include <limits>
#include <string>

class Camera;

class GameCamera : public Component {
    public:
        GameCamera(Camera *gameCamera);

        void start() override;
        void update(float dt) override;
        std::string getTypeName() const override { return "GameCamera"; }
        json toJson() const override;
        void fromJson(const json& j) override;

    private:
        GameObject* player;
        Camera*     gameCamera;
        float highestX = std::numeric_limits<float>::min();
        float undergroundYLevel = 0.0f;
        float cameraBuffer = 1.5f;
        float playerBuffer = 0.25f;

        glm::vec4 skeyColor = glm::vec4(92.0f / 255.0f, 148.0f / 255.0f, 252.0f / 255.0f, 1.0f);
        glm::vec4 undergroundColor = glm::vec4(0, 0, 0, 1);
};