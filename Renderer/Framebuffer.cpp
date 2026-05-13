#include <glad/glad.h>

#include "Framebuffer.h"
#include "Texture.h"
#include <iostream>

Framebuffer::Framebuffer(int width, int height) {
    this->width = width;
    this->height = height;

    // Create texture for the framebuffer
    texture = new Texture(width, height);

    // Generate framebuffer
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    // Attach texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);

    unsigned int renderbufferID;
    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferID);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
    // Delete framebuffer
    glDeleteFramebuffers(1, &framebufferID);
    delete texture;
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Framebuffer::getTextureID() const {
    return texture->getID();
}