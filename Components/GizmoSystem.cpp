#include "GizmoSystem.h"

GizmoSystem::GizmoSystem(Spritesheet* gizmoSpritesheet) {
    gizmos = gizmoSpritesheet;
}

void GizmoSystem::start() {
    // 在这里初始化你的Gizmo系统
    // gameObject->addComponent<SpriteRenderer>(gizmos);
}

void GizmoSystem::update(float dt) {
    // 在这里更新你的Gizmo系统
}