#pragma once

#include "graphics/objects/object.hpp"
#include "graphics/objects/mesh.hpp"

class Object2D : public Object {
    private:
        static Mesh quadMesh;

    public:
        Object2D(float x, float y, float width, float height);
        virtual void draw() const;
};
