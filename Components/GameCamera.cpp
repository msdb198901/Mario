#include "GameCamera.h"
#include "Camera.h"
#include "Window.h"

GameCamera::GameCamera(Camera *gameCamera) : gameCamera(gameCamera) {
}

void GameCamera::start() {
    //player = Window::getCurrentScene()->
    //Window::getCurrentScene()->getGameObjectWith<PlayerController>();
}

void GameCamera::update(float dt) {
    // glm::vec3 playerPos = player->getPosition();
    // float playerX = playerPos.x;
    // float playerY = playerPos.y;

    // if (playerX > highestX) {
    //     highestX = playerX;
    // }
}


json GameCamera::toJson() const {
    json j;
    j["type"] = getTypeName();
    return j;
}

void GameCamera::fromJson(const json& j) {
}