#pragma once

#include <glm/glm.hpp>

class Camera {
    public:
        Camera(glm::vec2 pos);
        void adjustProjection();

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        glm::mat4 getInverseViewMatrix();
        glm::mat4 getInverseProjectionMatrix();

        glm::vec2 getProjectionSize();
        float getZoom();
        void setZoom(float zoom);
        void addZoom(float zoom);

    private:
        glm::vec2 position;
        float zoom = 1.0f;
        glm::mat4 viewMatrix, projectionMatrix, inverseViewMatrix, inverseProjectionMatrix;

        float projectionWidth = 6, projectionHeight = 3;
        glm::vec4 clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec2 projectionSize = glm::vec2(projectionWidth, projectionHeight);
};