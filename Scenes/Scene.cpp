#include "Scene.h"
#include "LevelEditorSceneInitializer.h"
#include "SceneInitializer.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include <memory>

Scene::Scene(SceneInitializer* initializer) : initializer(initializer) {
}

void Scene::init() {
    if (initializer) {
        camera = new Camera(glm::vec2(0.0f, 0.0f));
        initializer->loadResources(this);
        initializer->init(this);
    }
}

void Scene::start() {
    isRunning = true;
    for (auto& gameObject : gameObjectList) {
        gameObject->start();
    }
    isRunning = true;
}

void Scene::addGameObjectToScene(GameObject* gameObject) {
    if (!isRunning) {
        gameObjectList.push_back(gameObject);
    } else {
        pendingObjectList.push_back(gameObject);
    }
    
}

void Scene::destroy() {
    // Destroy the scene
}

void Scene::render() {
    // Render the scene
}

void Scene::imgui() {
    if (initializer) {
        initializer->imgui();
    }
}

void Scene::load() {
    // Load the scene
}

void Scene::save() {
    // Save the scene
}

GameObject* Scene::createGameObject(const char* name) {
    GameObject* gameObject = new GameObject(name);
    gameObject->addComponent(std::make_unique<Transform>()); // 默认添加 Transform 组件
    return gameObject;
}

void Scene::destroyGameObject(const char* name) {
    // Destroy the game object with the given name
}

Camera* Scene::getCamera() {
    return camera;
}