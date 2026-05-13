#include "Scene.h"
#include "LevelEditorSceneInitializer.h"
#include "SceneInitializer.h"
#include "GameObject.h"
#include "Camera.h"

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
}

void Scene::addGameObjectToScene(GameObject* gameObject) {
    // Add the game object to the scene
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
    GameObject* gameObject = new GameObject();
    return gameObject;
}

void Scene::destroyGameObject(const char* name) {
    // Destroy the game object with the given name
}

Camera* Scene::getCamera() {
    return camera;
}