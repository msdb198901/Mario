#pragma once 

#include <vector>

class Texture;
class Sprite;
class Spritesheet {
    public:
        Spritesheet(Texture* texture, int spriteWidth, int spriteHeight, int numSprites, int spacing);
    private:
        std::vector<Sprite*> sprites;
        Texture* texture;
};