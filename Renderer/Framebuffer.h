#pragma once

class Texture;
class Framebuffer {

    public:
        Framebuffer(int width, int height);
        ~Framebuffer();

        void bind();
        void unbind();

        int getTextureID() const;
        int getFramebufferID() const { return framebufferID; }

    private:
        unsigned int framebufferID;
        Texture* texture;
        int width, height;
};