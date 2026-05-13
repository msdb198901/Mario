#include "AssetPool.h"

#include "Texture.h"
#include "Shader.h"
#include "Spritesheet.h"

#include <iostream>
#include <filesystem>
#include <cassert>
namespace fs = std::filesystem;

std::map<std::string, Texture*> AssetPool::textures;
std::map<std::string, Shader*> AssetPool::shaders;
std::map<std::string, Spritesheet*> AssetPool::spritesheets;

Texture* AssetPool::getTexture(std::string textureFilePath) {
    if (textures.find(textureFilePath) != textures.end()) {
        return textures[textureFilePath];
    }
    Texture* texture = new Texture();
    texture->init(textureFilePath);
    textures[textureFilePath] = texture;
    return texture;
}

Shader* AssetPool::getShader(std::string shaderFilePath) {
    if (shaders.find(shaderFilePath) != shaders.end()) {
        return shaders[shaderFilePath];
    }
    Shader* shader = new Shader(shaderFilePath.c_str());
    shader->compile();
    shaders[shaderFilePath] = shader;
    return shader;
}

Spritesheet* AssetPool::getSpritesheet(std::string spritesheetFilePath) {
    auto it = spritesheets.find(spritesheetFilePath.c_str());
    if (it == spritesheets.end()) {
        std::cerr << "Error: Tried to access spritesheet '" << spritesheetFilePath 
                    << "' and it has not been added to asset pool." << std::endl;
        assert(false);
    }
    return (it != spritesheets.end()) ? it->second : nullptr;
}

void AssetPool::addSpritesheet(std::string spritesheetFilePath, Spritesheet* spritesheet) {
    spritesheets[spritesheetFilePath] = spritesheet;
}

