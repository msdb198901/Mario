#pragma once 

#include "SceneInitializer.h"

class Scene;
class Spritesheet;
class GameObject;
class LevelEditorSceneInitializer : public SceneInitializer {
    public:
        virtual void init(Scene* scene) override;
        virtual void loadResources(Scene* scene) override;
        virtual void imgui() override;

    private:
        Spritesheet* sprites;
        GameObject* levelEditorStuff;
};