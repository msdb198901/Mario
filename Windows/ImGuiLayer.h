#pragma once

#include <GLFW/glfw3.h>

// Forward declarations
class GameViewWindow;
// class PropertiesWindow;
// class MenuBar;
// class SceneHierarchyWindow;
class Scene;
class PickingTexture;

class ImGuiLayer {
public:
    ImGuiLayer(GLFWwindow* window, PickingTexture* pickingTexture);
    ~ImGuiLayer();

    void initImGui();
    void update(float dt, Scene* currentScene);

    GameViewWindow* getGameViewWindow() const;
    // PropertiesWindow* getPropertiesWindow() const;

private:
    GLFWwindow* m_glfwWindow;
    PickingTexture* m_pickingTexture;

    // Sub‑windows (managed by this layer)
    GameViewWindow*      m_gameViewWindow;
    // PropertiesWindow*    m_propertiesWindow;
    // MenuBar*             m_menuBar;
    // SceneHierarchyWindow* m_sceneHierarchyWindow;

    void startFrame(float deltaTime);
    void endFrame();
    void setupDockspace();
    void destroyImGui();
};