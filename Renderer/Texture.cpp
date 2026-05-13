#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture() : textureID(0), width(0), height(0) {}

Texture::Texture(int width, int height) : textureID(0), width(width), height(height) {
    filePath = "Generated";
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::init(std::string filePath) {
    this->filePath = filePath;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Load image data here (e.g., using stb_image)
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, nullptr, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

