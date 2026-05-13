#include "PickingTexture.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

PickingTexture::PickingTexture(int width, int height) : pickingTextureId(0), framebufferID(0), depthBufferID(0) {
    init(width, height);
}

PickingTexture::~PickingTexture() {
    glDeleteTextures(1, &pickingTextureId);
    glDeleteFramebuffers(1, &framebufferID);
    glDeleteRenderbuffers(1, &depthBufferID);
}

bool PickingTexture::init(int width, int height) {
    // 1. 创建帧缓冲对象
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);


    // 2. 创建颜色附件纹理（存储物体 ID，整数格式）
    glGenTextures(1, &pickingTextureId);
    glBindTexture(GL_TEXTURE_2D, pickingTextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTextureId, 0);     

    // 3. 创建深度纹理
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &depthBufferID);
    glBindTexture(GL_TEXTURE_2D, depthBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferID, 0);

    // 4. 设置颜色写入目标（只渲染到颜色附件0）
    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);
    glReadBuffer(GL_NONE);  // 不需要从帧缓冲读取颜色，但可以保持

    // 5. 检查完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Picking framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    // 6. 解绑
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void PickingTexture::enableWriting() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
}

void PickingTexture::disableWriting() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int PickingTexture::readPixel(int x, int y) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    int pixel = 0;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
    return pixel;
}

std::vector<float> PickingTexture::readPixels(const glm::vec2& start, const glm::vec2& end) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    int width = static_cast<int>(end.x - start.x);
    int height = static_cast<int>(end.y - start.y);
    std::vector<float> pixels(width * height);
    glReadPixels(static_cast<int>(start.x), static_cast<int>(start.y), width, height, GL_RED_INTEGER, GL_UNSIGNED_INT, pixels.data());
    return pixels;
}