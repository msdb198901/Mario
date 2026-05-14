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
    getInstance()->gameViewportPos = pos;
}

void MouseListener::setGameViewportSize(glm::vec2 size) {
    getInstance()->gameViewportSize = size;
}

void MouseListener::endFrame() {
    getInstance()->scrollX = 0;
    getInstance()->scrollY = 0;
}

void MouseListener::clear() {
    getInstance()->scrollX = 0;
    getInstance()->scrollY = 0;
    getInstance()->xPos = 0;
    getInstance()->yPos = 0;
    getInstance()->lastX = 0;
    getInstance()->lastY = 0;
    getInstance()->mouseButtonDown = 0;
    getInstance()->m_isDragging = false;
    for (int i = 0; i < 9; i++) {
        getInstance()->mouseButtonPressed[i] = false;
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

    if (getInstance()->mouseButtonDown > 0) {
        getInstance()->m_isDragging = true;
    }

    getInstance()->lastX = getInstance()->xPos;
    getInstance()->lastY = getInstance()->yPos;
    getInstance()->lastWorldX = getInstance()->worldX;
    getInstance()->lastWorldY = getInstance()->worldY;

    getInstance()->xPos = xpos;
    getInstance()->yPos = ypos;
}

void MouseListener::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        getInstance()->mouseButtonDown |= 1 << button;
        if (button < 9) {
            getInstance()->mouseButtonPressed[button] = true;
        }
    } else if (action == GLFW_RELEASE) {
        getInstance()->mouseButtonDown &= ~(1 << button);
        if (button < 9) {
            getInstance()->mouseButtonPressed[button] = false;
            getInstance()->m_isDragging = false;
        }
    }
}

void MouseListener::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    getInstance()->scrollX += xoffset;
    getInstance()->scrollY += yoffset;
}

float MouseListener::getX() {
    return getInstance()->xPos;
}

float MouseListener::getY() {
    return getInstance()->yPos;
}

float MouseListener::getScrollX() {
    return getInstance()->scrollX;
}

float MouseListener::getScrollY() {
    return getInstance()->scrollY;
}

float MouseListener::getWorldX() {
    return getInstance()->lastWorldX - getInstance()->worldX;
}

float MouseListener::getWorldY() {
    return getInstance()->lastWorldY - getInstance()->worldY;
}

glm::vec2 MouseListener::getWorld() {
    return glm::vec2(getInstance()->lastWorldX - getInstance()->worldX, getInstance()->lastWorldY - getInstance()->worldY);
}

bool MouseListener::isDragging() {
    return getInstance()->m_isDragging;
}

bool MouseListener::isMouseButtonPressed(int button) {
    if (button < 0 || button >= 9) {
        return false;
    }
    return getInstance()->mouseButtonPressed[button];
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

