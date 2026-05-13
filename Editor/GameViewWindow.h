#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

class GameViewWindow {
public:
    GameViewWindow();
    ~GameViewWindow() = default;

    void imgui();
    bool getWantCaptureMouse() const;

private:
    bool m_isPlaying;
    bool m_windowIsHovered;

    glm::vec2 getLargestSizeForViewport() const;
    glm::vec2 getCenteredPositionForViewport(const glm::vec2& aspectSize) const;
};