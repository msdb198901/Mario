#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
class MouseListener {
    public:
        static void setGameViewportPos(glm::vec2 pos);
        static void setGameViewportSize(glm::vec2 size);

        static void endFrame();
        static void clear();
        static MouseListener* getInstance();
        static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        
        static float getX();
        static float getY();
        static float getScrollX();
        static float getScrollY();
        static float getWorldX();
        static float getWorldY();
        static glm::vec2 getWorld();

        static bool isDragging();
        static bool isMouseButtonPressed(int button);

        static glm::vec2 getScreenToWorld(const glm::vec2& screenCoords);
        static glm::vec2 getWorldToScreen(const glm::vec2& worldCoords);
        static float getScreenX();
        static float getScreenY();
        static glm::vec2 getScreen();

    private:
        MouseListener();

    private:
        static MouseListener* instance;
        float scrollX, scrollY;
        float xPos, yPos, lastX, lastY, worldX, worldY, lastWorldX, lastWorldY;
        bool mouseButtonPressed[9] = {false};
        bool m_isDragging;

        int mouseButtonDown = 0;
        glm::vec2 gameViewportPos;
        glm::vec2 gameViewportSize;
};