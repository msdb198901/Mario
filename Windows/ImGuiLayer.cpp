#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGuiLayer.h"
#include "GameViewWindow.h"
// #include "PropertiesWindow.h"
// #include "MenuBar.h"
// #include "SceneHierarchyWindow.h"
#include "Scene.h"
#include "Window.h"
// #include "KeyListener.h"
#include "MouseListener.h"
#include "PickingTexture.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fstream>

struct ImGuiContext {
    GameViewWindow*      gameViewWindow;
    // PropertiesWindow*    propertiesWindow;
};


ImGuiLayer::ImGuiLayer(GLFWwindow* window, PickingTexture* pickingTexture)
    : m_glfwWindow(window)
    , m_gameViewWindow(new GameViewWindow())
    // , m_propertiesWindow(new PropertiesWindow(pickingTexture))
    // , m_menuBar(new MenuBar())
    // , m_sceneHierarchyWindow(new SceneHierarchyWindow())
{
    ImGuiContext* context = new ImGuiContext();
    context->gameViewWindow = m_gameViewWindow;
    // context->propertiesWindow = m_propertiesWindow;
    glfwSetWindowUserPointer(window, context);
}

ImGuiLayer::~ImGuiLayer() {
    destroyImGui();
    // delete m_gameViewWindow;
    // delete m_propertiesWindow;
    // delete m_menuBar;
    // delete m_sceneHierarchyWindow;
}

void ImGuiLayer::initImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = "imgui.ini";            // We don't want to save .ini file
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.BackendPlatformName = "imgui_impl_glfw";

    // ---------- GLFW callbacks ----------
    // Keyboard
    glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        /*ImGuiIO& io = ImGui::GetIO();
        if (action == GLFW_PRESS)
            io.KeysDown[key] = true;
        else if (action == GLFW_RELEASE)
            io.KeysDown[key] = false;

        io.KeyCtrl  = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT]   || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt   = io.KeysDown[GLFW_KEY_LEFT_ALT]     || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]   || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        if (!io.WantCaptureKeyboard) {
            KeyListener::keyCallback(w, key, scancode, action, mods);
        }*/
    });

    // Character input
    glfwSetCharCallback(m_glfwWindow, [](GLFWwindow* w, unsigned int c) {
        if (c != GLFW_KEY_DELETE) {
            ImGui::GetIO().AddInputCharacter(c);
        }
    });

    // Mouse buttons
    glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* w, int button, int action, int mods) {
        ImGuiContext* context = (ImGuiContext*)glfwGetWindowUserPointer(w);

        ImGuiIO& io = ImGui::GetIO();
        bool mouseDown[5] = { false };
        if (button == GLFW_MOUSE_BUTTON_1 && action != GLFW_RELEASE) mouseDown[0] = true;
        if (button == GLFW_MOUSE_BUTTON_2 && action != GLFW_RELEASE) mouseDown[1] = true;
        if (button == GLFW_MOUSE_BUTTON_3 && action != GLFW_RELEASE) mouseDown[2] = true;
        if (button == GLFW_MOUSE_BUTTON_4 && action != GLFW_RELEASE) mouseDown[3] = true;
        if (button == GLFW_MOUSE_BUTTON_5 && action != GLFW_RELEASE) mouseDown[4] = true;

        io.MouseDown[0] = mouseDown[0];
        io.MouseDown[1] = mouseDown[1];
        io.MouseDown[2] = mouseDown[2];
        io.MouseDown[3] = mouseDown[3];
        io.MouseDown[4] = mouseDown[4];

        if (!io.WantCaptureMouse && mouseDown[1]) {
            ImGui::SetWindowFocus(nullptr);
        }

        if (context != nullptr && context->gameViewWindow && context->gameViewWindow->getWantCaptureMouse()) {
            MouseListener::mouseButtonCallback(w, button, action, mods);
        }
    });

    // Scroll
    glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* w, double xOffset, double yOffset) {
        ImGuiContext* context = (ImGuiContext*)glfwGetWindowUserPointer(w);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += (float)xOffset;
        io.MouseWheel   += (float)yOffset;
        if (!io.WantCaptureMouse || (context != nullptr && context->gameViewWindow && context->gameViewWindow->getWantCaptureMouse())) {
            MouseListener::mouseScrollCallback(w, xOffset, yOffset);
        } else {
            MouseListener::clear();
        }
    });

    // Clipboard
    io.SetClipboardTextFn = [](void* userData, const char* text) {
        glfwSetClipboardString((GLFWwindow*)userData, text);
    };
    io.GetClipboardTextFn = [](void* userData) -> const char* {
        const char* str = glfwGetClipboardString((GLFWwindow*)userData);
        return str ? str : "";
    };
    io.ClipboardUserData = m_glfwWindow;

    // ---------- Fonts ----------
    auto addFont = [&](const char* path, float sizePixels) {
        ImFontConfig fontConfig;
        fontConfig.GlyphRanges = io.Fonts->GetGlyphRangesDefault();
        fontConfig.PixelSnapH = true;
        io.Fonts->AddFontFromFileTTF(path, sizePixels, &fontConfig);
    };

    std::ifstream segoe("C:/Windows/Fonts/segoeui.ttf");
    if (segoe.good()) {
        addFont("C:/Windows/Fonts/segoeui.ttf", 32.0f);
    } else {
        std::ifstream cour("C:/Windows/Fonts/Cour.ttf");
        if (cour.good()) {
            addFont("C:/Windows/Fonts/Cour.ttf", 32.0f);
        }
    }

    // Initialize backends
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, false);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImGuiLayer::update(float dt, Scene* currentScene) {
    startFrame(dt);

    setupDockspace();
    if (currentScene) {
        currentScene->imgui();
    }
    // ImGui::ShowDemoWindow();
    m_gameViewWindow->imgui();
    // m_propertiesWindow->imgui();
    // m_sceneHierarchyWindow->imgui();

    endFrame();
}

void ImGuiLayer::startFrame(float /*deltaTime*/) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::endFrame() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::getWidth(), Window::getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backupCurrentContext);
}

void ImGuiLayer::destroyImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::setupDockspace() {
    int windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(mainViewport->WorkPos);
    ImGui::SetNextWindowSize(mainViewport->WorkSize);
    ImGui::SetNextWindowViewport(mainViewport->ID);
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2((float)Window::getWidth(), (float)Window::getHeight()));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Dockspace Demo", nullptr, windowFlags);
    ImGui::PopStyleVar(2);

    ImGui::DockSpace(ImGui::GetID("Dockspace"));

    // m_menuBar->imgui();

    ImGui::End();
}

GameViewWindow* ImGuiLayer::getGameViewWindow() const {
    return m_gameViewWindow;
}

// PropertiesWindow* ImGuiLayer::getPropertiesWindow() const {
//     return m_propertiesWindow;
// }