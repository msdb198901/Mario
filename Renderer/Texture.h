#pragma once

#include <string>

class Texture {
    public:
        Texture();
        Texture(int width, int height);

        void init(std::string filePath);

        void bind();
        void unbind();

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        std::string getFilePath() const { return filePath; }

        int getID() const { return textureID; }

        bool equals(const Texture& other) const {
            return textureID == other.textureID;
        }

        std::string getFilepath() const { return filePath; }

    private:
        unsigned int textureID;
        int width;
        int height;
        std::string filePath;
};