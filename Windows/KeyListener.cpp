#include "KeyListener.h"
#include <glfw/glfw3.h>

KeyListener* KeyListener::instance = nullptr;
std::unordered_map<int, bool> KeyListener::keyPressed;
std::unordered_map<int, bool> KeyListener::keyBeginPressed;

KeyListener* KeyListener::getInstance() {
    if (instance == nullptr) {
        instance = new KeyListener();
    }
    return instance;
}

KeyListener::KeyListener() {
    // Constructor code here (if needed)
}

void KeyListener::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        getInstance()->keyPressed[key] = true;
        getInstance()->keyBeginPressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        getInstance()->keyPressed[key] = false;
        getInstance()->keyBeginPressed[key] = false;
    }
}

bool KeyListener::isKeyDown(int key) {
    return getInstance()->keyPressed[key];
}

bool KeyListener::isKeyBeginPressed(int keyCode) {
    return getInstance()->keyBeginPressed[keyCode];
}

void KeyListener::endFrame() {
    for (auto& pair : getInstance()->keyBeginPressed) {
        pair.second = false;
    }
}
