#pragma once

#include "Component.h"

class Spritesheet;
class GizmoSystem : public Component {
public:
    GizmoSystem(Spritesheet* gizmoSpritesheet);

    void start() override;
    void update(float dt) override;
private:
    Spritesheet* gizmos;
    int usingGizmo = 0;
};