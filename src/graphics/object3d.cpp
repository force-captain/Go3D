#include "graphics/object3d.h"
#include "graphics/mesh.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

Object3D::Object3D(Mesh* meshPtr, glm::vec3 pos) {

}

void Object3D::draw(GLuint shaderProgram, glm::mat4& proj, glm::mat4& view) const {
    glUseProgram(shaderProgram);

    glm::mat4 model = getModelMatrix();
    glm::mat4 MVP = proj * view * model;

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, &MVP[0][0]);

    if (mat.textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mat.textureID);
    }
    glUniform3fv(glGetUniformLocation(shaderProgram, "uColor"), 1, &mat.colour[0]);

    mesh->draw();
}

glm::mat4 Object3D::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    // Translate
    model = glm::translate(model, position);

    // Rotate
    model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));

    // Scale
    model = glm::scale(model, scale);

    return model;
}
