#pragma once

#include <vector>

class Renderer;
class SpriteRenderer;
class Texture;
class GameObject;
class RenderBatch {

    public:
        RenderBatch(int maxBatchSize, int zIndex, Renderer* renderer);

        void start();
        void addSprite(SpriteRenderer* sprite);
        void render();
        bool destroyIfExists(GameObject* go);
        void loadVertexProperties(int index);
        std::vector<int> generateIndices();

        void loadElementIndices(std::vector<int> elements, int index);
        bool hasRoom() const;
        bool hasTextureRoom() const;
        bool hasTexture(Texture* texture) const;
        int zIndex() const { return _zIndex; }

        bool operator<(const RenderBatch& other) const {
            if (_zIndex != other._zIndex) return _zIndex < other._zIndex;
        }

    private:
        int _zIndex;
        std::vector<SpriteRenderer*> sprites;
        bool isRoom = false;
        float* vertices = nullptr;
        std::vector<int> texSlots = {0,1,2,3,4,5,6,7};

        std::vector<Texture*> textures;
        unsigned int vao = 0;
        unsigned int vbo = 0;
        int maxBatchSize = 1000;

        Renderer* renderer = nullptr;

    private:
        const int POS_SIZE = 2;
        const int COLOR_SIZE = 4;
        const int TEX_COORDS_SIZE = 2;
        const int TEX_ID_SIZE = 1;
        const int ENTITY_ID_SIZE = 1;

        const int POS_OFFSET = 0;
        const int COLOR_OFFSET = POS_OFFSET + POS_SIZE * sizeof(float);
        const int TEX_COORDS_OFFSET = COLOR_OFFSET + COLOR_SIZE * sizeof(float);
        const int TEX_ID_OFFSET = TEX_COORDS_OFFSET + TEX_COORDS_SIZE * sizeof(float);
        const int ENTITY_ID_OFFSET = TEX_ID_OFFSET + TEX_ID_SIZE * sizeof(float);
        const int VERTEX_SIZE = 10;
        const int VERTEX_SIZE_BYTES = VERTEX_SIZE * sizeof(float);
};