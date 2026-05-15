#include "LevelSceneInitializer.h"
#include "Scene.h"
#include "AssetPool.h"
#include "Spritesheet.h"
#include "Sprite.h"
#include "GameObject.h"
#include "imgui.h"
#include "GameCamera.h"

void LevelSceneInitializer::init(Scene* scene) {
    // Initialize the level editor specific components here.
    sprites = AssetPool::getSpritesheet("assets/images/spritesheets/decorationsAndBlocks.png");
    Spritesheet* gizmos = AssetPool::getSpritesheet("assets/images/gizmos.png");

    GameObject *cameraObject = scene->createGameObject("GameCamera");
    cameraObject->addComponent(std::make_unique<GameCamera>(scene->getCamera()));
    cameraObject->start();
    scene->addGameObjectToScene(cameraObject);
}

void LevelSceneInitializer::loadResources(Scene* scene) {
    // Load the level editor specific resources here.
    AssetPool::getShader("assets/shaders/default.glsl");
    AssetPool::addSpritesheet("assets/images/spritesheets/decorationsAndBlocks.png",
                new Spritesheet(AssetPool::getTexture("assets/images/spritesheets/decorationsAndBlocks.png"),
                        16, 16, 81, 0));
    AssetPool::addSpritesheet("assets/images/spritesheet.png",
            new Spritesheet(AssetPool::getTexture("assets/images/spritesheet.png"),
                    16, 16, 26, 0));
    AssetPool::addSpritesheet("assets/images/turtle.png",
            new Spritesheet(AssetPool::getTexture("assets/images/turtle.png"),
                    16, 24, 4, 0));
    AssetPool::addSpritesheet("assets/images/bigSpritesheet.png",
            new Spritesheet(AssetPool::getTexture("assets/images/bigSpritesheet.png"),
                    16, 32, 42, 0));
    AssetPool::addSpritesheet("assets/images/pipes.png",
            new Spritesheet(AssetPool::getTexture("assets/images/pipes.png"),
                    32, 32, 4, 0));
    AssetPool::addSpritesheet("assets/images/items.png",
            new Spritesheet(AssetPool::getTexture("assets/images/items.png"),
                    16, 16, 43, 0));
    AssetPool::addSpritesheet("assets/images/gizmos.png",
            new Spritesheet(AssetPool::getTexture("assets/images/gizmos.png"),
                    24, 48, 3, 0));
    AssetPool::getTexture("assets/images/blendImage2.png");
}

void LevelSceneInitializer::imgui() {
    // Implement the level editor specific ImGui interface here.
    //ImGui::Begin("Level Editor Stuff");

    //ImGui::End();
}

