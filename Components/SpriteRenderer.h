#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <string>

class Transform;
class Sprite;
class Texture;
class SpriteRenderer : public Component {
    public:
        SpriteRenderer();

        void start() override;
        void update(float dt) override;
        void imgui() override;


        void setDirty(bool dirty);
        bool getDirty() const ;
        void setClean() { isDirty = false; }

        glm::vec4 getColor() const;
        Texture* getTexture() const;

        glm::vec2* getTexCoords() const ;
        glm::vec2 getTexCoords(int index) const ;

        void setColor(const glm::vec4& color) ;
        void setSprite(Sprite* sprite);
        void setTexture(Texture* texture);

        json toJson() const override;
        void fromJson(const json& j) override;
        std::string getTypeName() const override { return "SpriteRenderer"; }

    private:
        glm::vec4 color = {1, 1, 1, 1};
        Sprite* sprite = nullptr;

        Transform* lastTransform = nullptr;
        bool isDirty = true; // 标记是否需要更新渲染数据
};  