#pragma once

struct Vector3 {
    float x;
    float y;
    float z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
};

enum class Colour {
    EMPTY,
    BLACK,
    WHITE
};

enum class MoveResult {
    OK,
    KO,
    SUICIDE,
    INVALID
};
