#pragma once

class Maths {
    public:
        static float toRadians(float degrees) { return degrees * 0.0174532925f; }
        static float toDegrees(float radians) { return radians * 57.2957795131f; }
};