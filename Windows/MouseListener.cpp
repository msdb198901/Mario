#include "MouseListener.h"
#include "Window.h"
#include "Camera.h"
#include "Scene.h"
#include "ImGuiLayer.h"
#include "GameViewWindow.h"

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>


MouseListener* MouseListener::instance = nullptr;

MouseListener::MouseListener() : scrollX(0), scrollY(0), xPos(0), yPos(0), lastX(0), lastY(0), worldX(0), worldY(0), lastWorldX(0), lastWorldY(0), m_isDragging(false) {
}


void MouseListener::setGameViewportPos(glm::vec2 pos) {
    instance->gameViewportPos = pos;
}

void MouseListener::setGameViewportSize(glm::vec2 size) {
    instance->gameViewportSize = size;
}

void MouseListener::endFrame() {
    instance->scrollX = 0;
    instance->scrollY = 0;
}

void MouseListener::clear() {
    instance->scrollX = 0;
    instance->scrollY = 0;
    instance->xPos = 0;
    instance->yPos = 0;
    instance->lastX = 0;
    instance->lastY = 0;
    instance->mouseButtonDown = 0;
    instance->m_isDragging = false;
    for (int i = 0; i < 9; i++) {
        instance->mouseButtonPressed[i] = false;
    }
}

MouseListener* MouseListener::getInstance() {
    if (instance == nullptr) {
        instance = new MouseListener();
    }
    return instance;
}

void MouseListener::mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!Window::RELEASE_BUILD) {
        if (!Window::getImGuiLayer()->getGameViewWindow()->getWantCaptureMouse()) {
            clear();
        }
    }

    if (instance->mouseButtonDown > 0) {
        instance->m_isDragging = true;
    }

    instance->lastX = instance->xPos;
    instance->lastY = instance->yPos;
    instance->lastWorldX = instance->worldX;
    instance->lastWorldY = instance->worldY;

    instance->xPos = xpos;
    instance->yPos = ypos;
}

void MouseListener::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        instance->mouseButtonDown |= 1 << button;
        if (button < 9) {
            instance->mouseButtonPressed[button] = true;
        }
    } else if (action == GLFW_RELEASE) {
        instance->mouseButtonDown &= ~(1 << button);
        if (button < 9) {
            instance->mouseButtonPressed[button] = false;
            instance->m_isDragging = false;
        }
    }
}

void MouseListener::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    instance->scrollX += xoffset;
    instance->scrollY += yoffset;
}

float MouseListener::getX() {
    return instance->xPos;
}

float MouseListener::getY() {
    return instance->yPos;
}

float MouseListener::getScrollX() {
    return instance->scrollX;
}

float MouseListener::getScrollY() {
    return instance->scrollY;
}

float MouseListener::getWorldX() {
    return instance->lastWorldX - instance->worldX;
}

float MouseListener::getWorldY() {
    return instance->lastWorldY - instance->worldY;
}

glm::vec2 MouseListener::getWorld() {
    return glm::vec2(instance->lastWorldX - instance->worldX, instance->lastWorldY - instance->worldY);
}

bool MouseListener::isDragging() {
    return instance->m_isDragging;
}

bool MouseListener::isMouseButtonPressed(int button) {
    if (button < 0 || button >= 9) {
        return false;
    }
    return instance->mouseButtonPressed[button];
}

glm::vec2 MouseListener::getScreenToWorld(const glm::vec2& screenCoords) {
    glm::vec2 normalizedScreenCoords = glm::vec2(screenCoords.x / Window::getWidth(), screenCoords.y / Window::getHeight());
    normalizedScreenCoords = normalizedScreenCoords * 2.0f - glm::vec2(1.0f);
    Camera* camera = Window::getCurrentScene()->getCamera();
    glm::vec4 tmp = glm::vec4(normalizedScreenCoords, 0.0f, 1.0f);
    glm::mat4 inverseProjection = camera->getInverseProjectionMatrix();
    glm::mat4 inverseView = camera->getInverseViewMatrix();

    tmp = inverseView * tmp;
    tmp = inverseProjection * tmp;
    return glm::vec2(tmp.x, tmp.y);
}

glm::vec2 MouseListener::getWorldToScreen(const glm::vec2& worldCoords) {
    Camera* camera = Window::getCurrentScene()->getCamera();
    glm::vec4 tmp = glm::vec4(worldCoords, 0.0f, 1.0f);
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    tmp = projection * view * tmp;
    tmp /= tmp.w; // 透视除法
    return glm::vec2((tmp.x + 1.0f) / 2.0f * Window::getWidth(), (tmp.y + 1.0f) / 2.0f * Window::getHeight());
}

