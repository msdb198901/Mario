
#pragma once 

#include "Texture.h"
#include "glm/glm.hpp"

class Sprite {
    public:
        Texture* getTexture() const { return texture; }
        float getWidth() const { return width; }
        float getHeight() const { return height; }
        glm::vec2* getTexCoords() const { return texCoords; }
        glm::vec2 getTexCoords(int index) const { return texCoords[index]; }
        int getTextureID() const { return texture ? texture->getID() : -1; }

        void setTexture(Texture* texture) { this->texture = texture; }
        void setWidth(float width) { this->width = width; }
        void setHeight(float height) { this->height = height; }
        void setTexCoords(glm::vec2* texCoords) { this->texCoords = texCoords; }
        void setTexCoords(glm::vec2 coords, int index) { this->texCoords[index] = coords; }

    private:
        float width, height;
        Texture* texture;
        glm::vec2* texCoords = new glm::vec2[4]{
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f}
        };
};