#include "Renderer.h"
#include "RenderBatch.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "Transform.h"

Shader* Renderer::currentShader = nullptr;

Renderer::Renderer() {
    batches.reserve(100); // 预留空间，避免频繁扩容
}

void Renderer::add(GameObject* go) {
    // 将 GameObject 添加到批次中
    if (go->getComponent<SpriteRenderer>()) {
        add(go->getComponent<SpriteRenderer>()); // 如果 GameObject 有 SpriteRenderer，则直接添加
    }
}

void Renderer::add(SpriteRenderer* sprite) {
    bool added = false;
    for (auto& batch : batches) {
        // 确保 zIndex 被计算过，避免重复计算影响性能
        if (batch->hasRoom() && batch->zIndex() == sprite->gameObject->transform->zIndex) {
            Texture* texture = sprite->getTexture();
            if (texture == nullptr || batch->hasTexture(texture) || batch->hasTextureRoom()) {
                batch->addSprite(sprite); // 将 SpriteRenderer 添加到批次中
                added = true;
                break;
            }
        }
    }

    if (!added) {
        RenderBatch* newBatch = new RenderBatch(MAX_BATCH_SIZE, sprite->gameObject->transform->zIndex, this); // 创建一个新的批次
        newBatch->start();
        batches.push_back(newBatch); 
        newBatch->addSprite(sprite); // 将 SpriteRenderer 添加到新批次中
        // 对新批次进行排序，确保渲染顺序正确
        std::sort(batches.begin(), batches.end());
    }
}

void Renderer::render() {
    currentShader->use(); // 使用当前绑定的 Shader
    for (auto& batch : batches) {
        batch->render(); // 渲染每个批次
    }
}

void Renderer::destroyGameObject(GameObject* go) {
    if (go->getComponent<SpriteRenderer>() == nullptr) return; // 如果 GameObject 没有 SpriteRenderer，则直接返回

    for (auto& batch : batches) {
        if (batch->destroyIfExists(go)) return; // 如果在批次中找到并删除了 GameObject，则返回
    }
}

void Renderer::bindShader(Shader* shader) {
    currentShader = shader; // 绑定当前使用的 Shader
}

Shader* Renderer::getBoundShader() {
    return currentShader; // 获取当前绑定的 Shader
}

