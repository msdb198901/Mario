#include "RenderBatch.h"
#include "Renderer.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Window.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera.h"
#include "Maths.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderBatch::RenderBatch(int maxBatchSize, int zIndex, Renderer* renderer) : maxBatchSize(maxBatchSize), _zIndex(zIndex), renderer(renderer) {
    vertices = new float[maxBatchSize * 4 * VERTEX_SIZE];
    sprites.reserve(maxBatchSize);
    isRoom = true;
}

void RenderBatch::start() {
    // 初始化 VAO、VBO 等 OpenGL 资源
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, maxBatchSize * 4 * VERTEX_SIZE * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    
    GLuint ebo;
    glGenBuffers(1, &ebo);
    auto indices = generateIndices();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxBatchSize * 6 * sizeof(int), indices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针 (位置、纹理坐标)
    glVertexAttribPointer(0, POS_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, (void*)POS_OFFSET);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, POS_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, (void*)COLOR_OFFSET);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, TEX_COORDS_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, (void*)TEX_COORDS_OFFSET);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, TEX_ID_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, (void*)TEX_ID_OFFSET);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, ENTITY_ID_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES, (void*)ENTITY_ID_OFFSET);
    glEnableVertexAttribArray(4);
}

void RenderBatch::addSprite(SpriteRenderer* sprite) {
    // 添加 SpriteRenderer 到批次，并更新顶点数据
    sprites.push_back(sprite);

    if (sprite->getTexture() != nullptr) {
        if (!hasTexture(sprite->getTexture())) {
            textures.push_back(sprite->getTexture());
        }
    }
    loadVertexProperties(sprites.size() - 1); // 更新顶点数据 (位置、颜色、纹理坐标等

    if ((int)sprites.size() >= maxBatchSize) {
        isRoom = false; // 批次已满，不能再添加精灵
    }
}

void RenderBatch::render() {
    // 绑定 VAO，设置纹理，绘制批次中的所有精灵
    bool rebufferData = false;

    for (int i = 0; i < (int)sprites.size(); ++i) {
        SpriteRenderer* sprite = sprites[i];
        if (sprite->getDirty()) {
            if (!hasTexture(sprite->getTexture())) {
                renderer->destroyGameObject(sprite->gameObject); // 销毁 GameObject
                renderer->add(sprite->gameObject); // 重新添加 GameObject 到渲染器 (可能需要排序或更新批次)
            } else {
                loadVertexProperties(i); // 更新顶点数据
                sprite->setClean();
                rebufferData = true; // 需要重新缓冲数据
            }
        }

        if (sprite->gameObject->transform->zIndex != zIndex()) {
            destroyIfExists(sprite->gameObject); // 重新排序或销毁 GameObject
            renderer->add(sprite->gameObject); // 重新添加 GameObject 到渲染器 (可能需要排序或更新批次)
            sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end()); // 从批次中移除精灵，并返回下一个迭代器位置
            --i;  // 关键：补偿 erase 后的索引偏移
        }
    }

    if (rebufferData) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, maxBatchSize * 4 * VERTEX_SIZE * sizeof(float), vertices);
    }

    Shader* shader = renderer->getBoundShader();
    shader->uploadMat4f("uProjection",  Window::getCurrentScene()->getCamera()->getProjectionMatrix());
    shader->uploadMat4f("uView", Window::getCurrentScene()->getCamera()->getViewMatrix());
    for (int i = 0; i < (int)textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        textures[i]->bind();
    }
    shader->uploadIntArray("uTextures", texSlots);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, 6 * sprites.size(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);

    for (int i = 0; i < (int)textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        textures[i]->unbind();
    }
    shader->detach(); // 分离着色器，以便其他批次可以使用不同的着色器
}

bool RenderBatch::destroyIfExists(GameObject* go) {
    // 从批次中移除与 GameObject 相关的 SpriteRenderer
    SpriteRenderer* sprite = go->getComponent<SpriteRenderer>();
    for (auto it = sprites.begin(); it != sprites.end(); ++it) {
        if ((*it)->gameObject == go) {
            sprites.erase(it);
            return true;
        }
    }
    return false;
}

void RenderBatch::loadVertexProperties(int index) {
    SpriteRenderer* sprite = sprites[index];

    int offset = index * 4 * VERTEX_SIZE;

    // 加载顶点属性到缓冲区
    glm::vec4 color = sprite->getColor();
    glm::vec2* texCoords = sprite->getTexCoords();

    int texId = 0;
    if (sprite->getTexture() != nullptr) {
        for (int i = 0; i < (int)textures.size(); ++i) {
            if (textures[i]->getID() == sprite->getTexture()->getID()) {
                texId = i + 1; // 加一是因为纹理单元从 GL_TEXTURE0 开始编号，而不是零
                break;
            }
        }
    }

    bool isRotated = sprite->gameObject->transform->rotation != 0;
    glm::mat4 model = glm::mat4(1.0f);
    if (isRotated) {
        model = glm::translate(model, glm::vec3(sprite->gameObject->transform->position, 0.0f));
        model = glm::rotate(model, Maths::toRadians(sprite->gameObject->transform->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(sprite->gameObject->transform->scale, 1.0f));
    }

    float xAdd = 0.5;
    float yAdd = 0.5;
    for (int i = 0; i < 4; ++i) {
        if (i == 1) {
            yAdd = -0.5;
        } else if (i == 2) {
            xAdd = -0.5;
        } else if (i == 3) {
            yAdd = 0.5;
        }

        glm::vec4 currentPos = glm::vec4(sprite->gameObject->transform->position.x + xAdd * sprite->gameObject->transform->scale.x, 
            sprite->gameObject->transform->position.y + yAdd * sprite->gameObject->transform->scale.y, 0.0f, 1.0f);
       
        if (isRotated) {
            currentPos = glm::vec4(model * glm::vec4(xAdd, yAdd, 0.0f, 1.0f));
        }

        vertices[offset] = currentPos.x;
        vertices[offset + 1] = currentPos.y;

        vertices[offset + 2] = color.r;
        vertices[offset + 3] = color.g;
        vertices[offset + 4] = color.b;
        vertices[offset + 5] = color.a;

        vertices[offset + 6] = texCoords[i].x;
        vertices[offset + 7] = texCoords[i].y;

        vertices[offset + 8] = (float)texId; // 纹理ID

        vertices[offset + 9] = (float)sprite->gameObject->getUid() + 1; // UID，用于剔除测试

        offset += VERTEX_SIZE; // 移动到下一个顶点
    }
}

std::vector<int> RenderBatch::generateIndices() {
    // 生成批次中的索引
    std::vector<int> indices = std::vector<int>(maxBatchSize * 6); // 每个精灵需要 6 个索引
    for (int i = 0; i < maxBatchSize; i++) {
        loadElementIndices(indices, i);
    }
    return indices;
}

void RenderBatch::loadElementIndices(std::vector<int> elements, int index) {
    int offsetArrayIndex = 6 * index;
    int offset = index * 4;
    // 3, 2, 0, 0, 2, 1        7, 6, 4, 4, 6, 5
    // 第一三角形
    elements[offsetArrayIndex + 0] = offset + 3;
    elements[offsetArrayIndex + 1] = offset + 2;
    elements[offsetArrayIndex + 2] = offset + 0;

    // 第二三角形
    elements[offsetArrayIndex + 3] = offset + 0;
    elements[offsetArrayIndex + 4] = offset + 2;
    elements[offsetArrayIndex + 5] = offset + 1;
}

bool RenderBatch::hasRoom() const {
    // 检查批次是否有空间添加新的 SpriteRenderer
    return isRoom;
}

bool RenderBatch::hasTextureRoom() const {
    // 检查批次是否有空间添加新的纹理
    return textures.size() < 7;
}

bool RenderBatch::hasTexture(Texture* texture) const {
    // 检查批次中是否已经包含指定的纹理
    for (const auto& tex : textures) {
        if (tex->getID() == texture->getID()) return true;
    }
    return false;
}