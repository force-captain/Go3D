#include "graphics/objects/object2d.hpp"
#include "graphics/objects/object.hpp"
#include "graphics/objects/mesh.hpp"

Mesh Object2D::quadMesh({
    {{0,0,0}, {0,0,1}, {1,1,1}, {0,0}},
    {{1,0,0}, {0,0,1}, {1,1,1}, {1,0}},
    {{1,1,0}, {0,0,1}, {1,1,1}, {1,1}},
    {{0,1,0}, {0,0,1}, {1,1,1}, {0,1}}
}, {
    0,1,2, 2,3,0
});

Object2D::Object2D(float x, float y, float width, float height) {
    mesh = quadMesh;

}
