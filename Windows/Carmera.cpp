#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>   // glm::lookAt, glm::translate, glm::rotate 等变换矩阵函数
#include <glm/ext/matrix_clip_space.hpp>  // glm::ortho, glm::perspective 等投影矩阵函数


Camera::Camera(glm::vec2 pos) : position(pos) {
    adjustProjection();
}

void Camera::adjustProjection() {
    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(0.0f, projectionSize.x * zoom, 0.0f, projectionSize.y * zoom, 0.0f, 100.0f);
    inverseViewMatrix = glm::inverse(viewMatrix);
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // 相机位置：Z = 20.0f（俯视视角）
    glm::vec3 cameraPos = glm::vec3(position, 20.0f);
    // 目标点：相机位置 + 前向方向（这样视线沿 Z 轴负方向）
    glm::vec3 cameraTarget = cameraPos + cameraFront;
    
    viewMatrix = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    inverseViewMatrix = glm::inverse(viewMatrix);
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}

glm::mat4 Camera::getInverseViewMatrix() {
    return inverseViewMatrix;
}

glm::mat4 Camera::getInverseProjectionMatrix() {
    return inverseProjectionMatrix;
}

glm::vec2 Camera::getProjectionSize() {
    return projectionSize;
}

float Camera::getZoom() {
    return zoom;
}

void Camera::setZoom(float zoom) {
    this->zoom = zoom;
    // adjustProjection();
}

void Camera::addZoom(float zoom) {
    this->zoom += zoom;
    // adjustProjection();
}