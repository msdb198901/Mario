#pragma once

class Scene;
class SceneInitializer {
    public:
        virtual void init(Scene* scene) = 0;
        virtual void loadResources(Scene* scene) = 0;
        virtual void imgui() = 0;
};