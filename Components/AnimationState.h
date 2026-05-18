#pragma once

#include <string>
#include <vector>

class Sprite;

struct Frame {
    Sprite* sprite;
    float frameTime;
}  ;

class AnimationState {
    public:
        void refreshTexture();
        void addFrame(Sprite* sprite, float frameTime);
        void addFrames(std::vector<Sprite*> sprites, float frameTime);
        void setLoop(bool doesLoop);

        void update(float dt);
        Sprite* getCurrentSprite();

    public:
        std::string title;
        std::vector<Frame> animationFrames;

    private:
        static Sprite* defaultSprite;
        float time = 0.0f;
        int currentSpriteIndex = 0;
        bool doesLoop = false;
};