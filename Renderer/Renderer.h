#pragma once

#include <vector>

class RenderBatch;
class Shader;
class GameObject;
class SpriteRenderer;

class Renderer {
    public:
        Renderer();

        void add(GameObject* go);
        void add(SpriteRenderer* sprite);
        void destroyGameObject(GameObject* go);

        static void bindShader(Shader* shader);
        static Shader* getBoundShader();
        void render();

    private:
        const int MAX_BATCH_SIZE = 1000;
        std::vector<RenderBatch*> batches;
        static Shader* currentShader;
};