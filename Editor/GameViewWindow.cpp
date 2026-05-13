#include "GameViewWindow.h"
#include "MouseListener.h"     // 假设存在 MouseListener::setGameViewportPos / setGameViewportSize
#include "Window.h"            // 假设存在 Window::getFramebuffer() / getTargetAspectRatio()
#include "EventSystem.h"       // 假设存在 EventSystem::notify
#include "Event.h"             // Event 类型定义
#include "Framebuffer.h"       // 假设存在 Framebuffer::getTextureId()


GameViewWindow::GameViewWindow()
    : m_isPlaying(false)
    , m_windowIsHovered(false)
{
}

void GameViewWindow::imgui() {
    ImGui::Begin("Game Viewport",
                 nullptr,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_MenuBar);

    // 菜单栏（Play / Stop）
    if (ImGui::BeginMenuBar()) {
        bool playEnabled = !m_isPlaying;
        if (ImGui::MenuItem("Play", "", m_isPlaying, playEnabled)) {
            m_isPlaying = true;
            EventSystem::notify(nullptr, new Event(EventType::GAME_START));
        }

        bool stopEnabled = m_isPlaying;
        if (ImGui::MenuItem("Stop", "", !m_isPlaying, stopEnabled)) {
            m_isPlaying = false;
            EventSystem::notify(nullptr, new Event(EventType::GAME_OVER));
        }
        ImGui::EndMenuBar();
    }

    // 居中放置视口纹理
    glm::vec2 windowSize = getLargestSizeForViewport();
    glm::vec2 windowPos  = getCenteredPositionForViewport(windowSize);
    ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y));

    int textureId = Window::getFramebuffer()->getTextureID();
    ImGui::ImageButton("GameViewportImage",
                        (void*)(intptr_t)textureId,
                       ImVec2(windowSize.x, windowSize.y),
                       ImVec2(0, 1),   // uv0
                       ImVec2(1, 0));  // uv1 (OpenGL 纹理原点在下，需要翻转 Y)

    m_windowIsHovered = ImGui::IsItemHovered();

    // 将视口信息传递给鼠标监听器，用于坐标转换
    // MouseListener::setGameViewportPos(ImVec2(windowPos.x + 10.0f, windowPos.y));
    // MouseListener::setGameViewportSize(windowSize);

    ImGui::End();
}

bool GameViewWindow::getWantCaptureMouse() const {
    return m_windowIsHovered;
}

glm::vec2 GameViewWindow::getLargestSizeForViewport() const {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    float aspectWidth  = windowSize.x;
    float aspectHeight = aspectWidth / Window::getTargetAspectRatio();

    if (aspectHeight > windowSize.y) {
        // 改为上下黑边（pillarbox）
        aspectHeight = windowSize.y;
        aspectWidth  = aspectHeight * Window::getTargetAspectRatio();
    }

    return glm::vec2(aspectWidth, aspectHeight);
}

glm::vec2 GameViewWindow::getCenteredPositionForViewport(const glm::vec2& aspectSize) const {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    float viewportX = (windowSize.x - aspectSize.x) * 0.5f;
    float viewportY = (windowSize.y - aspectSize.y) * 0.5f;

    return glm::vec2(viewportX + ImGui::GetCursorPosX(),
                     viewportY + ImGui::GetCursorPosY());
}