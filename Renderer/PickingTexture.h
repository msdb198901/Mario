#pragma once

#include <vector>
#include <glm/glm.hpp>

class PickingTexture {
    public:
        PickingTexture(int width, int height);
        ~PickingTexture();

        bool init(int width, int height);

        void enableWriting();
        void disableWriting();

        int readPixel(int x, int y);
        std::vector<float> readPixels(const glm::vec2& start, const glm::vec2& end);

    private:
        unsigned int pickingTextureId;
        unsigned int framebufferID;
        unsigned int depthBufferID;
        unsigned int fboId;
};