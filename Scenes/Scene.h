#pragma once

#include <vector>

class SceneInitializer;
class GameObject;
class Camera;
class Scene {
    public:
        Scene() = default;
        Scene(SceneInitializer* initializer);
        virtual ~Scene() = default;

        void init();
        void start();
        void addGameObjectToScene(GameObject* gameObject);
        void destroy();

        void render();
        void imgui();

        void load();
        void save();

        GameObject* createGameObject(const char* name);
        void destroyGameObject(const char* name);
        Camera* getCamera();

    private:
        bool isRunning = false;
        SceneInitializer* initializer;
        std::vector<GameObject*> gameObjectList;
        std::vector<GameObject*> pendingObjectList; 
        Camera* camera;
};