#include "Spritesheet.h"
#include "Sprite.h"
#include "glm/vec2.hpp"
#include "Texture.h"
#include <vector>
#include <iostream>

Spritesheet::Spritesheet(Texture* texture, int spriteWidth, int spriteHeight, int numSprites, int spacing) {
    this->texture = texture;
    int currentX = 0;
    int currentY = texture->getHeight() - spriteHeight; // Start from the top-left corner
    for (int i = 0; i < numSprites; i++) {
        float topY = static_cast<float>(currentY + spriteHeight) / texture->getHeight();
        float bottomY = static_cast<float>(currentY) / texture->getHeight();
        float rightX = static_cast<float>(currentX + spriteWidth) / texture->getWidth();
        float leftX = static_cast<float>(currentX) / texture->getWidth();

        glm::vec2 texCoords[4] = {
            {rightX, topY},
            {rightX, bottomY},
            {leftX, bottomY},
            {leftX, topY}
        };

        Sprite* sprite = new Sprite();
        sprite->setTexture(texture);
        sprite->setWidth(spriteWidth);
        sprite->setHeight(spriteHeight);
        sprite->setTexCoords(texCoords);
        sprites.push_back(sprite);

        currentX += (spriteWidth + spacing);
        if (currentX >= texture->getWidth()) {
            currentX = 0;
            currentY -= (spriteHeight + spacing);
        }
    }
}