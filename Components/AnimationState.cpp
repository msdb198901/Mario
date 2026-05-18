#include "AnimationState.h"
#include "Sprite.h"
#include "AssetPool.h"

Sprite* AnimationState::defaultSprite = new Sprite();

void AnimationState::refreshTexture() {
    for (Frame& frame : animationFrames) {
        frame.sprite->setTexture(AssetPool::getTexture(frame.sprite->getTexture()->getFilepath()));
    }
}

void AnimationState::addFrame(Sprite* sprite, float duration) {
    animationFrames.push_back({sprite, duration});
}

void AnimationState::addFrames(std::vector<Sprite*> sprites, float duration) {
    for (Sprite* sprite : sprites) {
        addFrame(sprite, duration);
    }
}

void AnimationState::setLoop(bool loop) {
    this->doesLoop = loop;
}

void AnimationState::update(float deltaTime) {
   if (currentSpriteIndex < (int)animationFrames.size()) {
       time -= deltaTime;
       if (time <= 0) {
           if (!(currentSpriteIndex == animationFrames.size() - 1 && !doesLoop)) {
               currentSpriteIndex = (currentSpriteIndex + 1) % animationFrames.size();
           }
           time = animationFrames[currentSpriteIndex].frameTime;
       }
   }
}

Sprite* AnimationState::getCurrentSprite() {
    if (currentSpriteIndex < 0 || currentSpriteIndex >= (int)animationFrames.size()) {
        return defaultSprite;
    }
    return animationFrames[currentSpriteIndex].sprite;
}