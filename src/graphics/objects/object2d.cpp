#include "graphics/objects/object2d.hpp"
#include "graphics/objects/object.hpp"
#include "graphics/objects/mesh.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

std::shared_ptr<Mesh> Object2D::createQuadMesh() {
    static std::shared_ptr<Mesh> quadMesh = std::make_shared<Mesh>(
        std::vector<Vertex>{{{0,0,0}, {0,0,1}, {1,1,1}, {0,0}},
    {{1,0,0}, {0,0,1}, {1,1,1}, {1,0}},
    {{1,1,0}, {0,0,1}, {1,1,1}, {1,1}},
    {{0,1,0}, {0,0,1}, {1,1,1}, {0,1}}
}, std::vector<unsigned int>{
    0,1,2, 2,3,0
});
    return quadMesh;
}

Object2D::Object2D(float x, float y, float width, float height) : position(x, y), rotation(0.0f), scale(width, height), mesh(createQuadMesh()) {

}

glm::mat4 Object2D::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    return model;
}

void Object2D::update(float deltaTime){}
