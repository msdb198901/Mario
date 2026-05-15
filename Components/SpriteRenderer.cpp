#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Texture.h"
#include "Transform.h"
#include "GameObject.h"
#include "AssetPool.h"

json SpriteRenderer::toJson() const {
    json j;
    j["color"] = {color.r, color.g, color.b, color.a};
    if (sprite && sprite->getTexture()) {
        j["textureID"] = sprite->getTexture()->getID();
        j["width"] = sprite->getWidth();
        j["height"] = sprite->getHeight();
        j["texCoords"] = {
            {sprite->getTexCoords(0).x, sprite->getTexCoords(0).y},
            {sprite->getTexCoords(1).x, sprite->getTexCoords(1).y},
            {sprite->getTexCoords(2).x, sprite->getTexCoords(2).y},
            {sprite->getTexCoords(3).x, sprite->getTexCoords(3).y}
        };
    }
    return j;
}

void SpriteRenderer::fromJson(const json& j) {
    color = glm::vec4(j["color"][0], j["color"][1], j["color"][2], j["color"][3]);
    if (sprite) {
        Texture* texture = AssetPool::getTexture(j["textureID"]);
        float width = j["width"];
        float height = j["height"];
        glm::vec2 texCoords[4] = {
            {j["texCoords"][0][0], j["texCoords"][0][1]},
            {j["texCoords"][1][0], j["texCoords"][1][1]},
            {j["texCoords"][2][0], j["texCoords"][2][1]},
            {j["texCoords"][3][0], j["texCoords"][3][1]}
        };
        sprite->setTexture(texture);
        sprite->setWidth(width);
        sprite->setHeight(height);
        sprite->setTexCoords(texCoords);
    }
}

SpriteRenderer::SpriteRenderer() : Component() {
}

void SpriteRenderer::start() {
    if (sprite && sprite->getTexture()) {
        sprite->setTexture(AssetPool::getTexture(sprite->getTexture()->getFilePath()));
    }
    lastTransform = gameObject->getComponent<Transform>();
}

void SpriteRenderer::update(float dt) {
    Transform* currentTransform = gameObject->getComponent<Transform>();
    if (currentTransform != lastTransform) {
        setDirty(true);
        lastTransform = currentTransform;
    }
}

void SpriteRenderer::imgui() {
    if  (JImGui::colorPicker4("Color", color)) {
        setDirty(true);
    }
}

void SpriteRenderer::setDirty(bool dirty) {
    isDirty = dirty;
}

bool SpriteRenderer::getDirty() const {
    return isDirty;
}

glm::vec4 SpriteRenderer::getColor() const {
    return color;
}

Texture* SpriteRenderer::getTexture() const {
    return sprite ? sprite->getTexture() : nullptr;
}

glm::vec2* SpriteRenderer::getTexCoords() const {
    return sprite ? sprite->getTexCoords() : nullptr;
}
glm::vec2 SpriteRenderer::getTexCoords(int index) const {
    return sprite ? sprite->getTexCoords(index) : glm::vec2(0, 0);
}

void SpriteRenderer::setColor(const glm::vec4& color) {
    if (this->color != color) {
        setDirty(true);
        this->color = color;
    }
}

void SpriteRenderer::setSprite(Sprite* sprite) {
    this->sprite = sprite;
}

void SpriteRenderer::setTexture(Texture* texture) {
    if (sprite == nullptr) {
        sprite = new Sprite();
    }
    sprite->setTexture(texture);
}

