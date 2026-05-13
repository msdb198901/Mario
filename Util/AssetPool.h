#pragma once

#include <map>
#include <string>

class Texture;
class Shader;
class Spritesheet;

class AssetPool {
    public:
        static Texture* getTexture(std::string textureFilePath);
        static Shader* getShader(std::string shaderFilePath);
        static Spritesheet* getSpritesheet(std::string spritesheetFilePath);
        static void addSpritesheet(std::string spritesheetFilePath, Spritesheet* spritesheet);

    private:
        static std::map<std::string, Texture*> textures;
        static std::map<std::string, Shader*> shaders;
        static std::map<std::string, Spritesheet*> spritesheets;
};